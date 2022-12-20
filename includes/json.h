#ifndef JSON_H
#define JSON_H

#include <stdbool.h>
#include <stdio.h>

#include "lexer.h"

typedef struct json
{
    struct token *token;
    struct json **children;
    size_t children_count;
} json;

json *json_parse(char *json);

json *json_parse_file(FILE *file);

char *json_get_value(struct json *object, char *key);

bool json_get_bool(struct json *object, char *key);

int json_get_int(struct json *object, char *key);

double json_get_double(struct json *object, char *key);

float json_get_float(struct json *object, char *key);

void json_free(struct json *object);

#endif // !JSON_H
