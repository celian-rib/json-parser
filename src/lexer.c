#include "lexer.h"

#include <ctype.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct state
{
    char *cursor;
    size_t line;
    size_t column;
    struct token *look_ahead;
};

static struct state state = {
    .cursor = NULL,
    .line = 1,
    .column = 1,
    .look_ahead = NULL,
};

static void increment_cursor()
{
    if (*state.cursor == '\n')
    {
        state.line++;
        state.column = 1;
    }
    else
    {
        state.column++;
    }
    state.cursor++;
}

static void skip_whitespace()
{
    while (isspace(*state.cursor))
        increment_cursor();
}

static struct token *create_non_terminal_token(enum TOKEN_TYPE type)
{
    struct token *token = malloc(sizeof(struct token));
    token->type = type;
    token->value = malloc(2);
    token->value[0] = *state.cursor;
    token->value[1] = '\0';
    state.look_ahead = token;
    return token;
}

static struct token *generate_next_token()
{
    struct token *token = malloc(sizeof(struct token));
    token->type = TOKEN_TYPE_NONE;
    token->value = NULL;

    skip_whitespace();

    if (*state.cursor == '\0')
    {
        struct token *token = create_non_terminal_token(TOKEN_TYPE_EOF);
        increment_cursor();
        return token;
    }

    if (*state.cursor == '{')
    {
        token->type = TOKEN_TYPE_LEFT_BRACE;
        token->value = malloc(2);
        token->value[0] = *state.cursor;
        token->value[1] = '\0';
        token->line = state.line;
        token->column = state.column;
        increment_cursor();
        return token;
    }

    if (*state.cursor == '}')
    {
        token->type = TOKEN_TYPE_RIGHT_BRACE;
        token->value = malloc(2);
        token->value[0] = *state.cursor;
        token->value[1] = '\0';
        token->line = state.line;
        token->column = state.column;
        increment_cursor();
        return token;
    }

    if (*state.cursor == '[')
    {
        token->type = TOKEN_TYPE_LEFT_BRACKET;
        token->value = malloc(2);
        token->value[0] = *state.cursor;
        token->value[1] = '\0';
        token->line = state.line;
        token->column = state.column;
        increment_cursor();
        return token;
    }

    if (*state.cursor == ']')
    {
        token->type = TOKEN_TYPE_RIGHT_BRACKET;
        token->value = malloc(2);
        token->value[0] = *state.cursor;
        token->value[1] = '\0';
        token->line = state.line;
        token->column = state.column;
        increment_cursor();
        return token;
    }

    if (*state.cursor == ',')
    {
        token->type = TOKEN_TYPE_COMMA;
        token->value = malloc(2);
        token->value[0] = *state.cursor;
        token->value[1] = '\0';
        token->line = state.line;
        token->column = state.column;
        increment_cursor();
        return token;
    }

    if (*state.cursor == ':')
    {
        token->type = TOKEN_TYPE_COLON;
        token->value = malloc(2);
        token->value[0] = *state.cursor;
        token->value[1] = '\0';
        token->line = state.line;
        token->column = state.column;
        increment_cursor();
        return token;
    }

    if (*state.cursor == '"')
    {
        token->type = TOKEN_TYPE_STRING;
        char *start = ++state.cursor;

        while (*state.cursor != '"')
            increment_cursor();

        token->value = malloc(state.cursor - start + 1);
        memcpy(token->value, start, state.cursor - start);

        token->value[state.cursor - start] = '\0';

        token->line = state.line;
        token->column = state.column;
        increment_cursor();
        return token;
    }

    if (*state.cursor == 't' && *(state.cursor + 1) == 'r'
        && *(state.cursor + 2) == 'u' && *(state.cursor + 3) == 'e')
    {
        token->type = TOKEN_TYPE_BOOLEAN;
        token->value = (char *)true;
        state.cursor += 4;
        state.column += 4;
        return token;
    }

    if (*state.cursor == 'f' && *(state.cursor + 1) == 'a'
        && *(state.cursor + 2) == 'l' && *(state.cursor + 3) == 's'
        && *(state.cursor + 4) == 'e')
    {
        token->type = TOKEN_TYPE_BOOLEAN;
        token->value = (char *)false;
        state.cursor += 5;
        state.column += 5;
        return token;
    }

    if (*state.cursor == 'n' && *(state.cursor + 1) == 'u'
        && *(state.cursor + 2) == 'l' && *(state.cursor + 3) == 'l')
    {
        token->type = TOKEN_TYPE_NULL;
        token->value = NULL;
        state.cursor += 4;
        state.column += 4;
        return token;
    }

    if (*state.cursor == '-' || (*state.cursor >= '0' && *state.cursor <= '9'))
    {
        token->type = TOKEN_TYPE_NUMBER;
        char *start = state.cursor;

        while (*state.cursor >= '0' && *state.cursor <= '9')
            increment_cursor();

        if (*state.cursor == '.')
        {
            increment_cursor();

            while (*state.cursor >= '0' && *state.cursor <= '9')
                increment_cursor();
        }

        token->value = malloc(state.cursor - start + 1);
        memcpy(token->value, start, state.cursor - start);

        token->value[state.cursor - start] = '\0';

        return token;
    }
    return NULL;
}

void lexer_init(char *json)
{
    state.cursor = json;
    state.look_ahead = generate_next_token();
}

struct token *eat_token(enum TOKEN_TYPE type)
{
    if (get_token_type() != type)
    {
        errx(1, "Error: Invalid token type. Expected %c, got %c (l:%zu, c:%zu)",
             type, state.look_ahead->type, state.look_ahead->line,
             state.look_ahead->column);
        return NULL;
    }

    struct token *token = state.look_ahead;
    state.look_ahead = generate_next_token();
    return token;
}

struct token *get_token()
{
    return state.look_ahead;
}

enum TOKEN_TYPE get_token_type()
{
    if (state.look_ahead == NULL)
        return TOKEN_TYPE_NONE;
    return state.look_ahead->type;
}

void free_token(struct token *token)
{
    free(token->value);
    free(token);
}
