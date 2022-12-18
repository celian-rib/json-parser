CC = gcc
CFLAGS = -Wall

SRC = lexer/lexer.c parser/parser.c main.c object/json_object.c

all: main

main: $(SRC)
	$(CC) $(CFLAGS) -o $@ $^
