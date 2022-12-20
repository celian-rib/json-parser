#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "json_object.h"
#include "parser.h"

static void print_tree(struct json_object *node)
{
    if (node->children_count == 0)
        return;

    printf("=> %s\n", node->token->value);

    for (size_t i = 0; i < node->children_count; i++)
        printf("--> %s\n", node->children[i]->token->value);

    for (size_t i = 0; i < node->children_count; i++)
        print_tree(node->children[i]);
}

int main(int argc, char *argv[])
{
    char *buffer = malloc(1024);
    size_t buffer_size = 1024;
    size_t buffer_length = 0;
    int c;
    while ((c = getchar()) != EOF)
    {
        if (buffer_length + 1 >= buffer_size)
        {
            buffer_size *= 2;
            buffer = realloc(buffer, buffer_size);
        }
        buffer[buffer_length++] = c;
    }
    buffer[buffer_length] = '\0';

    json_object *json = parse_json(buffer);

    printf("Parsing successful!\n");
    print_tree(json);

    if (argc > 1)
    {
        char *value = json_get_value(json, argv[1]);
        if (value)
            printf("Result: %s\n", value);
        else
            printf("Result not found\n");
    }

    free_json_object(json);
}
