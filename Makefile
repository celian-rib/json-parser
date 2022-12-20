CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -fPIC -fno-builtin -Wvla -I includes
LDFLAGS = -shared

TARGET_LIB = libjson.so

SRC_OBJS= src/lexer.o src/parser.o src/json.o 

TESTS_OBJS = tests/test_json.o tests/test_lexer.o

all: $(TARGET_LIB) tests

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_LIB): $(SRC_OBJS)
	$(CC) -shared -o $@ $^

check: CFLAGS += -I /opt/homebrew/Cellar/criterion/2.4.1_1/include/
check: $(TARGET_LIB) $(TESTS_OBJS)
	$(CC) -o $@ $^ -L. -ljson -L /opt/homebrew/lib/ -lcriterion -fsanitize=address -g
	./check

main: example/main.o $(TARGET_LIB)
	$(CC) -o $@ $^ -L. -ljson

clean:
	rm -f $(TARGET_LIB) $(SRC_OBJS) $(TESTS_OBJS) check main example/main.o

.PHONY: all clean check
