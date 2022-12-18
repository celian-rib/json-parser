#ifndef JSON_OBJECT_H
#define JSON_OBJECT_H

#include "../lexer/lexer.h"

struct json_object
{
    struct token *token;
    struct json_object **children;
    size_t children_count;
};

char *get_value(struct json_object *object, char *key);

#endif // !JSON_OBJECT_H
