#ifndef JSON_OBJECT_H
#define JSON_OBJECT_H

#include <stdbool.h>

#include "lexer.h"

typedef struct json_object
{
    struct token *token;
    struct json_object **children;
    size_t children_count;
} json_object;

char *json_get_value(struct json_object *object, char *key);

bool json_get_bool(struct json_object *object, char *key);

int json_get_int(struct json_object *object, char *key);

double json_get_double(struct json_object *object, char *key);

float json_get_float(struct json_object *object, char *key);

void free_json_object(struct json_object *object);

#endif // !JSON_OBJECT_H
