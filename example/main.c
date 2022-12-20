#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "json.h"

int main(int argc, char *argv[])
{
    if (argc < 2) 
    {
        fprintf(stderr, "Usage: %s <json file> <query>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    json *json = json_parse_file(file);

    printf("Parsing successful!\n");

    if (argc > 1)
    {
        char *value = json_get_value(json, argv[2]);
        if (value)
            printf("Result: %s\n", value);
        else
            printf("Result is null\n");
    }

    json_free(json);
}
