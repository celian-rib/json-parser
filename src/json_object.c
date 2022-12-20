#include "json_object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "string.h"

char *get_value_rec(struct json_object *object, char **path_ptr)
{
    if (object == NULL)
        return NULL;

    if (path_ptr == NULL)
        return NULL;

    char *key = strtok_r(NULL, ".[", path_ptr);

    if (key == NULL)
        return NULL;

    if (object->token->type == TOKEN_TYPE_LEFT_BRACE)
    {
        for (size_t i = 0; i < object->children_count; i++)
        {
            struct json_object *child = object->children[i];
            if (strcmp(child->token->value, key) == 0)
            {
                if (*path_ptr == NULL)
                    return child->children[0]->token->value;
                return get_value_rec(child->children[0], path_ptr);
            }
        }

        return NULL; // Not found
    }
    else if (object->token->type == TOKEN_TYPE_LEFT_BRACKET)
    {
        size_t index = atoi(key);
        if (index >= object->children_count)
            return NULL; // Out of bounds

        struct json_object *child = object->children[index];

        if (child->token->type == TOKEN_TYPE_LEFT_BRACE
            || child->token->type == TOKEN_TYPE_LEFT_BRACKET)
        {
            if (*path_ptr == NULL)
                return NULL; // No key or index specified
            return get_value_rec(child, path_ptr);
        }

        if (*path_ptr == NULL)
            return child->token->value;
        return get_value_rec(child, path_ptr);
    }

    // TODO if no child error

    if (*path_ptr == NULL)
        return object->children[0]->token->value;

    return NULL; // More path to go but no children
}

char *get_value(struct json_object *object, char *path)
{
    if (object == NULL)
        return NULL;

    if (path == NULL)
        return object->token->value;
    if (path[0] == '\0')
        return object->token->value;

    char *path_ptr = strdup(path);
    char *result = get_value_rec(object, &path_ptr);
    free(path_ptr);
    return result;
}

bool get_bool(struct json_object *object, char *key)
{
    return strcmp(get_value(object, key), "1") == 0;
}

int get_int(struct json_object *object, char *key)
{
    return atoi(get_value(object, key));
}

double get_double(struct json_object *object, char *key)
{
    return atof(get_value(object, key));
}

float get_float(struct json_object *object, char *key)
{
    return atof(get_value(object, key));
}

void free_json_object(struct json_object *object)
{
    if (object == NULL)
        return;

    free_token(object->token);

    for (size_t i = 0; i < object->children_count; i++)
    {
        free_json_object(object->children[i]);
    }

    free(object->children);
    free(object);
}
