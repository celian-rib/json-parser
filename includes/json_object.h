#ifndef JSON_OBJECT_H
#define JSON_OBJECT_H

#include <stdbool.h>

#include "lexer.h"

struct json_object
{
    struct token *token;
    struct json_object **children;
    size_t children_count;
};

char *get_value(struct json_object *object, char *key);

bool get_bool(struct json_object *object, char *key);

int get_int(struct json_object *object, char *key);

double get_double(struct json_object *object, char *key);

float get_float(struct json_object *object, char *key);

void free_json_object(struct json_object *object);

#endif // !JSON_OBJECT_H
