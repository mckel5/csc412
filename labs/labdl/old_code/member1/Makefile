CFLAGS = -Wall -Wextra -fsanitize=address
CFLAGS += -g
.PHONY: all clean

all: pwd

clean:
	rm -f pwd

main:
	$(CC) $(CFLAGS) $^ -o $@
