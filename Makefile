CC = gcc
CPPFLAGS = -D_DEFAULT_SOURCE
CFLAGS = -Wall -Wextra -std=c99 -fPIC -fno-builtin -Wvla -I includes
LDFLAGS = -shared
VPATH = src

TARGET_LIB = libjson.so

SRC_OBJS= lexer.o parser.o json_object.o 
OBJS = main.o $(SRC_OBJS)

TESTS_OBJS = $(SRC_OBJS) tests/test_json.o tests/test_lexer.o

all: library

library: $(TARGET_LIB)
$(TARGET_LIB): CFLAGS += -pedantic -fvisibility=hidden -Werror
$(TARGET_LIB): LDFLAGS += -Wl
$(TARGET_LIB): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

dev: CFLAGS += -fsanitize=address -g
dev: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

check: CFLAGS += -fsanitize=address -lcriterion
check: CFLAGS += -L /opt/homebrew/lib/
check: CFLAGS += -I /opt/homebrew/Cellar/criterion/2.4.1_1/include/
check: $(TESTS_OBJS)
	clang-format --dry-run --Werror src/*.c
	clang-format --dry-run --Werror tests/*.c
	clang-format --dry-run --Werror includes/*.h
	$(CC) $(CFLAGS) -o test_suite $^
	./test_suite
	make clean
	make

clean:
	$(RM) $(TARGET_LIB) $(OBJS) dev debug test_suite src/*.o tests/*.o

.PHONY: all $(TARGET_LIB) clean dev check check_mac
