#include <stdio.h>
#include <stdlib.h>

#include "parser/parser.h"

int main(void)
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

    parse_json(buffer);
}
