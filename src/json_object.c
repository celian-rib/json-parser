#include "json_object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "string.h"

char *get_value(struct json_object *object, char *path)
{
    if (object == NULL)
        return NULL;

    char *path_ptr = NULL;
    char *key = strtok_r(path, ".[]", &path_ptr);

    if (object->token->type == TOKEN_TYPE_LEFT_BRACE)
    {
        for (size_t i = 0; i < object->children_count; i++)
        {
            struct json_object *child = object->children[i];
            if (strcmp(child->token->value, key) == 0)
            {
                if (path_ptr == NULL)
                    return child->children[0]->token->value;
                return get_value(child->children[0], path_ptr);
            }
        }

        return NULL; // Not found
    }
    else if (object->token->type == TOKEN_TYPE_LEFT_BRACKET)
    {
        size_t index = atoi(key);
        if (index >= object->children_count)
            return NULL; // Out of bounds

        printf("index: %zu\n", index);
        struct json_object *child = object->children[index];

        if (child->token->type == TOKEN_TYPE_LEFT_BRACE
            || child->token->type == TOKEN_TYPE_LEFT_BRACKET)
        {
            if (path_ptr == NULL)
                return NULL; // No key or index specified
            return get_value(child, path_ptr);
        }
        else
        {
            if (path_ptr == NULL)
                return child->token->value;
            return NULL; // No key specified
        }
        return get_value(child, path_ptr);
    }

    // TODO if no chil d error

    if (path_ptr == NULL)
        return object->children[0]->token->value;

    return NULL; // More path to go but no children
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
