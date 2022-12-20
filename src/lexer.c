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

static void increment_cursor_for_char(char c)
{
    if (*state.cursor != c)
    {
        errx(1, "Expected '%c' at line %zu, column %zu", c, state.line,
             state.column);
    }
    increment_cursor();
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
    token->line = state.line;
    token->column = state.column;
    state.look_ahead = token;

    if (type != TOKEN_TYPE_EOF)
        increment_cursor();
    return token;
}

static struct token *create_string_token()
{
    struct token *token = malloc(sizeof(struct token));
    token->type = TOKEN_TYPE_STRING;
    token->line = state.line;
    token->column = state.column;

    increment_cursor();
    char *start = state.cursor;
    bool escaped = false;

    while (*state.cursor != '"' || escaped)
    {
        escaped = false;
        if (*state.cursor == '\\')
            escaped = true;
        increment_cursor();
    }

    token->value = malloc(state.cursor - start + 1);
    memcpy(token->value, start, state.cursor - start);

    token->value[state.cursor - start] = '\0';

    increment_cursor();
    return token;
}

static struct token *create_true_token()
{
    struct token *token = malloc(sizeof(struct token));
    token->type = TOKEN_TYPE_BOOLEAN;
    token->line = state.line;
    token->column = state.column;
    token->value = malloc(2);
    token->value[0] = '1';
    token->value[1] = '\0';

    increment_cursor_for_char('t');
    increment_cursor_for_char('r');
    increment_cursor_for_char('u');
    increment_cursor_for_char('e');

    return token;
}

static struct token *create_false_token()
{
    struct token *token = malloc(sizeof(struct token));
    token->type = TOKEN_TYPE_BOOLEAN;
    token->value = malloc(2);
    token->value[0] = '0';
    token->value[1] = '\0';
    token->line = state.line;
    token->column = state.column;

    increment_cursor_for_char('f');
    increment_cursor_for_char('a');
    increment_cursor_for_char('l');
    increment_cursor_for_char('s');
    increment_cursor_for_char('e');

    return token;
}

static struct token *create_null_token()
{
    struct token *token = malloc(sizeof(struct token));
    token->type = TOKEN_TYPE_NULL;
    token->value = NULL;
    token->line = state.line;
    token->column = state.column;

    increment_cursor_for_char('n');
    increment_cursor_for_char('u');
    increment_cursor_for_char('l');
    increment_cursor_for_char('l');

    return token;
}

static struct token *create_number_token()
{
    struct token *token = malloc(sizeof(struct token));
    token->type = TOKEN_TYPE_NUMBER;
    token->line = state.line;
    token->column = state.column;

    char *start = state.cursor;

    bool negative = false;
    bool decimal = false;

    while (isdigit(*state.cursor) || *state.cursor == '-'
           || *state.cursor == '.')
    {
        if (*state.cursor == '-')
        {
            if (negative)
                errx(1, "Unexpected '-' at line %zu, column %zu", state.line,
                     state.column);
            negative = true;
        }
        else if (*state.cursor == '.')
        {
            if (decimal)
                errx(1, "Unexpected '.' at line %zu, column %zu", state.line,
                     state.column);
            decimal = true;
        }
        increment_cursor();
    }

    token->value = malloc(state.cursor - start + 1);
    memcpy(token->value, start, state.cursor - start);
    token->value[state.cursor - start] = '\0';

    return token;
}

static struct token *generate_next_token()
{
    struct token *token = NULL;

    skip_whitespace();

    if (*state.cursor == '\0')
        token = create_non_terminal_token(TOKEN_TYPE_EOF);
    else if (*state.cursor == '{')
        token = create_non_terminal_token(TOKEN_TYPE_LEFT_BRACE);
    else if (*state.cursor == '}')
        token = create_non_terminal_token(TOKEN_TYPE_RIGHT_BRACE);
    else if (*state.cursor == '[')
        token = create_non_terminal_token(TOKEN_TYPE_LEFT_BRACKET);
    else if (*state.cursor == ']')
        token = create_non_terminal_token(TOKEN_TYPE_RIGHT_BRACKET);
    else if (*state.cursor == ',')
        token = create_non_terminal_token(TOKEN_TYPE_COMMA);
    else if (*state.cursor == ':')
        token = create_non_terminal_token(TOKEN_TYPE_COLON);
    else if (*state.cursor == '"')
        token = create_string_token();
    else if (*state.cursor == 't')
        token = create_true_token();
    else if (*state.cursor == 'f')
        token = create_false_token();
    else if (*state.cursor == 'n')
        token = create_null_token();
    else if (isdigit(*state.cursor) || *state.cursor == '-')
        token = create_number_token();

    return token;
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
