#include "json.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "string.h"

char *get_value_rec(struct json *object, char **path_ptr)
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
            struct json *child = object->children[i];
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

        struct json *child = object->children[index];

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

    if (*path_ptr == NULL)
    {
        if (object->children_count == 0)
            return NULL;
        return object->children[0]->token->value;
    }

    return NULL; // More path to go but no children
}

char *json_get_value(struct json *object, char *path)
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

bool json_get_bool(struct json *object, char *key)
{
    return strcmp(json_get_value(object, key), "1") == 0;
}

int json_get_int(struct json *object, char *key)
{
    return atoi(json_get_value(object, key));
}

double json_get_double(struct json *object, char *key)
{
    return atof(json_get_value(object, key));
}

float json_get_float(struct json *object, char *key)
{
    return atof(json_get_value(object, key));
}

void json_free(struct json *object)
{
    if (object == NULL)
        return;

    free_token(object->token);

    for (size_t i = 0; i < object->children_count; i++)
    {
        json_free(object->children[i]);
    }

    free(object->children);
    free(object);
}

json *json_parse_file(FILE *file)
{
    if (file == NULL)
        return NULL;

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(size + 1);
    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    json *result = json_parse(buffer);

    free(buffer);

    return result;
}
