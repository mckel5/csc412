#define DMALLOC_DISABLE 1
#include "dmalloc.hh"

#include <cassert>
#include <cstring>
#include <unordered_set>

/*
DMALLOC BLOCK LAYOUT

<metadata> <alignment padding> <boundary check> <USABLE MEMORY> <boundary check>
 32 B       4 B                 4 B              dynamic         4 B
*/

// Statistics updated by dmalloc and dfree
struct dmalloc_stats statistics = {
    .nactive = 0,
    .active_size = 0,
    .ntotal = 0,
    .total_size = 0,
    .nfail = 0,
    .fail_size = 0,
    .heap_min = __UINTPTR_MAX__,
    .heap_max = 0,
};

std::unordered_set<uintptr_t> all_allocated_addresses;
std::unordered_set<uintptr_t> active_addresses;
std::unordered_set<uintptr_t> freed_addresses;

const int boundary_check_value = 526304725;
const size_t boundary_check_value_size = sizeof(boundary_check_value);
const size_t metadata_size = sizeof(block_metadata);

// Padding to ensure pointers returned by dmalloc are divisible by 16
const int alignment_padding = 4;

// Helper function declarations
uintptr_t get_containing_block_address(uintptr_t address);
block_metadata get_metadata(uintptr_t data_start_address);
void set_metadata(uintptr_t data_start_address, block_metadata metadata);

/**
 * dmalloc(sz,file,line)
 *      malloc() wrapper. Dynamically allocate the requested amount `sz` of memory and
 *      return a pointer to it
 *
 * @arg size_t sz : the amount of memory requested
 * @arg const char *file : a string containing the filename from which dmalloc was called
 * @arg long line : the line number from which dmalloc was called
 *
 * @return a pointer to the heap where the memory was reserved
 */
void* dmalloc(size_t sz, const char* file, long line) {
    (void)file, (void)line;  // avoid uninitialized variable warnings

    // Prevent oversized allocations (fixes sanitizer error)
    if (sz >= 0x10000000000) {
        statistics.nfail++;
        statistics.fail_size += sz;
        return nullptr;
    }

    // Allocate space for metadata and data
    void* base_malloc_result = base_malloc(sz + metadata_size + (boundary_check_value_size * 2) + alignment_padding);

    // Update statistics
    statistics.ntotal++;
    statistics.total_size += sz;

    // Handle failed allocation
    if (base_malloc_result == nullptr) {
        statistics.nfail++;
        statistics.fail_size += sz;
        return nullptr;
    }

    // Calculate address that will be available to caller
    uintptr_t data_address = reinterpret_cast<uintptr_t>(base_malloc_result) + metadata_size + boundary_check_value_size + alignment_padding;

    // If applicable, remove address from freed addresses list
    if (freed_addresses.find(data_address) != freed_addresses.end()) {
        freed_addresses.erase(data_address);
    }

    // Update statistics
    statistics.nactive++;
    statistics.active_size += sz;
    if (data_address < statistics.heap_min) statistics.heap_min = data_address;
    if (data_address > statistics.heap_max) statistics.heap_max = data_address + sz;

    all_allocated_addresses.insert(data_address);
    active_addresses.insert(data_address);

    // Add metadata to newly-allocated block
    block_metadata metadata = {
        .addr = data_address,
        .size = sz,
        .file = file,
        .line = line,
    };
    memcpy(base_malloc_result, &metadata, metadata_size);

    // Set boundary check values
    int* boundary_l = reinterpret_cast<int*>(data_address - boundary_check_value_size);
    int* boundary_r = reinterpret_cast<int*>(data_address + sz);
    *boundary_l = boundary_check_value;
    *boundary_r = boundary_check_value;

    // Return pointer to usable data section
    return reinterpret_cast<void*>(data_address);
}

/**
 * dfree(ptr, file, line)
 *      free() wrapper. Release the block of heap memory pointed to by `ptr`. This should
 *      be a pointer that was previously allocated on the heap. If `ptr` is a nullptr do nothing.
 *
 * @arg void *ptr : a pointer to the heap
 * @arg const char *file : a string containing the filename from which dfree was called
 * @arg long line : the line number from which dfree was called
 */
void dfree(void* ptr, const char* file, long line) {
    (void)file, (void)line;  // avoid uninitialized variable warnings

    // dfree(nullptr) should not do anything
    if (ptr == nullptr) return;

    uintptr_t provided_address = reinterpret_cast<uintptr_t>(ptr);

    // Check if pointer exists in heap
    if (provided_address < statistics.heap_min || provided_address > statistics.heap_max) {
        fprintf(stderr, "MEMORY BUG: %s:%lu: invalid free of pointer %p, not in heap\n", file, line, ptr);
        abort();
    }

    // Check if block has previously been allocated
    if (all_allocated_addresses.find(provided_address) == all_allocated_addresses.end()) {
        fprintf(stderr, "MEMORY BUG: %s:%lu: invalid free of pointer %p, not allocated\n", file, line, ptr);

        // Check if address resides within an existing block on the heap
        if (uintptr_t containing_address = get_containing_block_address(provided_address)) {
            block_metadata metadata = get_metadata(containing_address);
            fprintf(stderr, "%s:%lu: %p is %lu bytes inside a %lu byte region allocated here\n", file, line - 1, ptr, provided_address - containing_address, metadata.size);
        }

        abort();
    }

    // Retrieve metadata from block
    block_metadata metadata = get_metadata(provided_address);

    // Check if block has already been freed
    if (freed_addresses.find(provided_address) != freed_addresses.end()) {
        fprintf(stderr, "MEMORY BUG: %s:%lu: invalid free of pointer %p, double free\n", file, line, ptr);
        abort();
    }

    // Check if boundary values were overwritten
    int boundary_l = *reinterpret_cast<int*>(provided_address - boundary_check_value_size);
    int boundary_r = *reinterpret_cast<int*>(provided_address + metadata.size);
    if (boundary_l != boundary_check_value || boundary_r != boundary_check_value) {
        fprintf(stderr, "MEMORY BUG: %s:%lu: detected wild write during free of pointer %p\n", file, line, ptr);
        abort();
    }

    // Mark block as freed
    active_addresses.erase(provided_address);
    freed_addresses.insert(provided_address);

    // Update statistics
    statistics.active_size -= metadata.size;
    statistics.nactive--;

    // Free memory
    void* metadata_p = reinterpret_cast<void*>(provided_address - metadata_size - boundary_check_value_size - alignment_padding);
    base_free(metadata_p);
}

/**
 * dcalloc(nmemb, sz, file, line)
 *      calloc() wrapper. Dynamically allocate enough memory to store an array of `nmemb`
 *      number of elements with wach element being `sz` bytes. The memory should be initialized
 *      to zero
 *
 * @arg size_t nmemb : the number of items that space is requested for
 * @arg size_t sz : the size in bytes of the items that space is requested for
 * @arg const char *file : a string containing the filename from which dcalloc was called
 * @arg long line : the line number from which dcalloc was called
 *
 * @return a pointer to the heap where the memory was reserved
 */
void* dcalloc(size_t nmemb, size_t sz, const char* file, long line) {
    // Terminate if arguments cause an integer overflow
    if ((nmemb * sz) / sz != nmemb) {
        statistics.nfail++;
        return nullptr;
    }

    // Allocate memory
    void* ptr = dmalloc(nmemb * sz, file, line);

    // Fill block with zeroes
    if (ptr) {
        memset(ptr, 0, nmemb * sz);
    }

    return ptr;
}

/**
 * drealloc(ptr, sz, file, line)
 *      Reallocate the dynamic memory pointed to by `ptr` to hold at least
 *      `sz` bytes, returning a pointer to the new block. If `ptr` is
 *      `nullptr`, behaves like `dmalloc(sz, file, line)`. If `sz` is 0,
 *      behaves like `dfree(ptr, file, line)`. The allocation request
 *      was at location `file`:`line`.
 *
 * @arg void *ptr : a pointer to the heap
 * @arg size_t sz : the amount of memory requested
 * @arg const char *file : a string containing the filename from which drealloc was called
 * @arg long line : the line number from which drealloc was called
 *
 * @return a pointer to the heap where the memory was reserved
 */
void* drealloc(void* ptr, size_t sz, const char* file, long line) {
    if (ptr == nullptr) {
        return dmalloc(sz, file, line);
    }

    if (!sz) {
        dfree(ptr, file, line);
        return nullptr;
    }

    bool collision_will_occur = false;
    uintptr_t resized_block_start_address = reinterpret_cast<uintptr_t>(ptr) - metadata_size - boundary_check_value_size - alignment_padding;
    uintptr_t resized_block_end_address = reinterpret_cast<uintptr_t>(ptr) + sz + boundary_check_value_size;

    // Check if resizing this block will cause a collision with any other blocks
    for (auto address : active_addresses) {
        uintptr_t block_start_address = address - metadata_size - boundary_check_value_size - alignment_padding;
        collision_will_occur = (resized_block_start_address <= block_start_address && block_start_address <= resized_block_end_address);
    }

    block_metadata* metadata = reinterpret_cast<block_metadata*>(resized_block_start_address);

    if (collision_will_occur) {
        // Copy data to new, larger block and free this block
        void* new_ptr = dmalloc(sz, file, line);
        memcpy(new_ptr, ptr, metadata->size);
        free(ptr);
        return new_ptr;
    }

    // If no collision will occur, resize the current block
    metadata->size = sz;
    // Push the boundary check value back to account for the new size
    void* boundary_check_r = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(ptr) + metadata->size);
    void* new_boundary_check_r = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(ptr) + sz);
    memcpy(new_boundary_check_r, boundary_check_r, boundary_check_value_size);

    return ptr;
}

/**
 * get_statistics(stats)
 *      fill a dmalloc_stats pointer with the current memory statistics
 *
 * @arg dmalloc_stats *stats : a pointer to the the dmalloc_stats struct we want to fill
 */
void get_statistics(dmalloc_stats* stats) {
    memcpy(stats, &statistics, sizeof(statistics));
}

/**
 * print_statistics()
 *      print the current memory statistics to stdout
 */
void print_statistics() {
    dmalloc_stats stats;
    get_statistics(&stats);

    printf("alloc count: active %10llu   total %10llu   fail %10llu\n",
           stats.nactive, stats.ntotal, stats.nfail);
    printf("alloc size:  active %10llu   total %10llu   fail %10llu\n",
           stats.active_size, stats.total_size, stats.fail_size);
}

/**
 * print_leak_report()
 *      Print a report of all currently-active allocated blocks of dynamic
 *      memory.
 */
void print_leak_report() {
    for (const auto& address : all_allocated_addresses) {
        auto metadata = reinterpret_cast<block_metadata*>(address - boundary_check_value_size - alignment_padding - metadata_size);

        if (freed_addresses.find(address) == freed_addresses.end()) {
            printf("LEAK CHECK: %s:%lu: allocated object %p with size %lu\n",
                   metadata->file,
                   metadata->line,
                   reinterpret_cast<void*>(metadata->addr),
                   metadata->size);
        }
    }
}

/**
 * get_containing_block_address(address)
 *      Return the data start address of the enclosing memory block,
 *      if one exists. Otherwise, return 0.
 */
uintptr_t get_containing_block_address(uintptr_t address) {
    for (uintptr_t start_address : all_allocated_addresses) {
        block_metadata metadata = get_metadata(start_address);
        if (freed_addresses.find(address) != freed_addresses.end()) continue;
        uintptr_t end_address = start_address + metadata.size;
        if (start_address <= address && address <= end_address) return start_address;
    }

    return 0;
}

/**
 * get_metadata(data_start_address)
 *      Get a block's metadata based on the data address
 *      (the address given to the user)
 */
block_metadata get_metadata(uintptr_t data_start_address) {
    return *reinterpret_cast<block_metadata*>(data_start_address - metadata_size - boundary_check_value_size - alignment_padding);
}

/**
 * set_metadata(data_start_address)
 *      Update a block's metadata based on the data address
 *      (the address given to the user)
 */
void set_metadata(uintptr_t data_start_address, block_metadata metadata) {
    block_metadata* metadata_p = reinterpret_cast<block_metadata*>(data_start_address - metadata_size - boundary_check_value_size - alignment_padding);
    *metadata_p = metadata;
}