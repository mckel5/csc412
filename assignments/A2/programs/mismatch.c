#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE (1024 * 1024)  // 1 MiB

// HASHSET HEADERS -->
typedef size_t (*hash_func_t)(char*, size_t);
struct hashset_st;
typedef struct hashset_st* hashset_t;
hashset_t hashset_create(void);
void hashset_destroy(hashset_t set);
void hashset_set_hash_function(hashset_t set, hash_func_t func);
void hashset_clean(hashset_t set);
size_t hashset_num_items(hashset_t set);
int hashset_add(hashset_t set, char* key, size_t key_len);
int hashset_remove(hashset_t set, char* key, size_t key_len);
int hashset_is_member(hashset_t set, char* key, size_t key_len);
// <-- END HASHSET HEADERS

hashset_t load_dictionary();

int main() {
    // Load dictionary
    hashset_t dictionary = load_dictionary();

    // Initialize buffer
    char buffer[BUFFER_SIZE];

    // Initialize mispelled words array
    // "BUFFER_SIZE / 2" prevents segfaults
    char* mispelled_words[BUFFER_SIZE / 2];
    int mispelled_word_count = 0;

    // Read data from stdin, compare against dictionary
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        int len = strlen(buffer);

        // Discard trailing newline
        if (buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }

        // Save word iff not in dictionary
        if (!hashset_is_member(dictionary, buffer, len)) {
            char* word = malloc(len + 1);
            strncpy(word, buffer, len + 1);
            mispelled_words[mispelled_word_count++] = word;
        }
    }

    // Free dictionary
    hashset_destroy(dictionary);

    // Print mispelled words then free array
    for (int i = 0; i < mispelled_word_count - 1; i++) {
        printf("%s\n", mispelled_words[i]);
        free(mispelled_words[i]);
    }

    printf("%s", mispelled_words[mispelled_word_count - 1]);
    free(mispelled_words[mispelled_word_count - 1]);

    return 0;
}

hashset_t load_dictionary() {
    // Open dictionary file
    FILE* file;
    char filename[] = "unix_dict.text";
    file = fopen(filename, "r");

    // Initialize buffer
    char buffer[BUFFER_SIZE];

    // Initialize hashset
    hashset_t set = hashset_create();

    // Add each word to hashset
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        int len = strlen(buffer);

        // Discard trailing newline
        if (buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }

        hashset_add(set, buffer, len);
    }

    return set;
}

// HASHSET FUNCTIONS
// Published on GitHub by Erhan Barış (https://github.com/erhanbaris/simple_hash)

/* ############# DEFINITIONS ############# */

static const size_t prime_1 = 73;
static const size_t prime_2 = 5009;

struct hashset_item_st {
    size_t hash;
    char* item;
};

struct hashset_st {
    size_t nbits;
    size_t mask;

    size_t capacity;
    struct hashset_item_st* items;
    size_t nitems;
    size_t n_deleted_items;

    hash_func_t hash_func;
};

size_t hash_function(char* p, size_t len) {
    size_t hash = 0;
    for (; *p; ++p)
        hash ^= *p + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    return hash;
}

/* ############# HASHSET ############# */

hashset_t hashset_create() {
    hashset_t set = (hashset_t)calloc(1, sizeof(struct hashset_st));

    if (set == NULL) {
        return NULL;
    }

    set->hash_func = hash_function;
    set->nbits = 3;
    set->capacity = (size_t)(1 << set->nbits);
    set->mask = set->capacity - 1;
    set->items = (struct hashset_item_st*)calloc(set->capacity, sizeof(struct hashset_item_st));
    if (set->items == NULL) {
        hashset_destroy(set);
        return NULL;
    }
    set->nitems = 0;
    set->n_deleted_items = 0;
    return set;
}

void hashset_clean(hashset_t set) {
    set->nitems = 0;
    set->n_deleted_items = 0;

    size_t i = 0;
    while (i != set->capacity)
        set->items[i++].hash = 0;
}

size_t hashset_num_items(hashset_t set) {
    return set->nitems;
}

void hashset_destroy(hashset_t set) {
    if (set) {
        free(set->items);
    }
    free(set);
}

void hashset_set_hash_function(hashset_t set, hash_func_t func) {
    set->hash_func = func;
}

static int hashset_add_member(hashset_t set, char* key, size_t hash) {
    size_t index;

    if (hash < 2) {
        return -1;
    }

    index = set->mask & (prime_1 * hash);

    while (set->items[index].hash != 0 && set->items[index].hash != 1) {
        if (set->items[index].hash == hash) {
            return 0;
        } else {
            /* search free slot */
            index = set->mask & (index + prime_2);
        }
    }

    ++set->nitems;
    if (set->items[index].hash == 1) {
        --set->n_deleted_items;
    }

    set->items[index].hash = hash;
    set->items[index].item = key;
    return 1;
}

static void set_maybe_rehash(hashset_t set) {
    struct hashset_item_st* old_items;
    size_t old_capacity, index;

    if (set->nitems + set->n_deleted_items >= (double)set->capacity * 0.85) {
        old_items = set->items;
        old_capacity = set->capacity;
        ++set->nbits;
        set->capacity = (size_t)(1 << set->nbits);
        set->mask = set->capacity - 1;
        set->items = (struct hashset_item_st*)calloc(set->capacity, sizeof(struct hashset_item_st));
        set->nitems = 0;
        set->n_deleted_items = 0;
        assert(set->items);
        for (index = 0; index < old_capacity; ++index) {
            hashset_add_member(set, old_items[index].item, old_items[index].hash);
        }
        free(old_items);
    }
}

int hashset_add(hashset_t set, char* key, size_t key_len) {
    size_t hash = set->hash_func(key, key_len);
    int rv = hashset_add_member(set, key, hash);
    set_maybe_rehash(set);
    return rv;
}

int hashset_remove(hashset_t set, char* key, size_t key_len) {
    size_t hash = set->hash_func(key, key_len);
    size_t index = set->mask & (prime_1 * hash);

    while (set->items[index].hash != 0) {
        if (set->items[index].hash == hash) {
            set->items[index].hash = 1;
            --set->nitems;
            ++set->n_deleted_items;
            return 1;
        } else {
            index = set->mask & (index + prime_2);
        }
    }
    return 0;
}

int hashset_is_member(hashset_t set, char* key, size_t key_len) {
    size_t hash = set->hash_func(key, key_len);
    size_t index = set->mask & (prime_1 * hash);

    while (set->items[index].hash != 0) {
        if (set->items[index].hash == hash) {
            return 1;
        } else {
            index = set->mask & (index + prime_2);
        }
    }
    return 0;
}
