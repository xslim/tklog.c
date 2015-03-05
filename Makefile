CC ?= cc
VALGRIND ?= valgrind
CFLAGS = -std=c99 -Wall -Wextra -I.
LDFLAGS =""
VALGRIND_OPTS ?= --leak-check=full
SRC = $(wildcard *.c)


OBJS = $(SRC:.c=.o)
TESTS = $(wildcard test*.c)

test: $(TESTS)
	@rm -rf *.o
	@$(CC) $(CFLAGS) $(SRC) -o test
	@./test


%.o: %.c
	$(CC) $< -c -o $@ $(CFLAGS)

clean:
	rm -rf *.o
# rm -f example $(OBJS)
# rm -rf test/fixtures

.PHONY: test $(TESTS)
#clean #grind
