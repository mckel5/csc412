CFLAGS = -Wall -Wextra -fsanitize=address
CFLAGS += -g
.PHONY: all clean

all: makewords

clean:
	rm -f makewords

main:
	$(CC) $(CFLAGS) $^ -o $@
