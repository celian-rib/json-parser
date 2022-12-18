#include "json_object.h"

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
        for (int i = 0; i < object->children_count; i++)
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
        int index = atoi(key);
        if (index >= object->children_count)
            return NULL; // Out of bounds

        printf("index: %d\n", index);
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
