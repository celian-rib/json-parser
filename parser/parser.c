#include "parser.h"

#include <stdbool.h>
#include <stdio.h>

#include "../lexer/lexer.h"

static void parse_value();
static void parse_list();
static void parse_dict();
static void parse_pair();

static bool is_value(struct token *token)
{
    return token->type == TOKEN_TYPE_NUMBER //
        || token->type == TOKEN_TYPE_STRING //
        || token->type == TOKEN_TYPE_NULL //
        || token->type == TOKEN_TYPE_BOOLEAN;
}

void parse_json(char *json)
{
    lexer_init(json);

    if (is_value(get_token()))
    {
        parse_value();
    }
    else if (get_token()->type == TOKEN_TYPE_LEFT_BRACE)
    {
        parse_dict();
    }
    else if (get_token()->type == TOKEN_TYPE_LEFT_BRACKET)
    {
        parse_list();
    }
    else
    {
        printf("Error: Invalid JSON input (l:%zu, c:%zu)\n", get_token()->line,
               get_token()->column);
        return;
    }

    eat_token(TOKEN_TYPE_EOF);

    printf("Valid JSON input\n");
}

static void parse_value()
{
    if (!is_value(get_token()))
    {
        printf("Error: Invalid value : %s (l:%zu, c:%zu)\n", get_token()->value,
               get_token()->line, get_token()->column);
        return;
    }

    if (get_token()->type == TOKEN_TYPE_LEFT_BRACE)
        parse_dict();
    else if (get_token()->type == TOKEN_TYPE_LEFT_BRACKET)
        parse_list();
    else if (get_token()->type == TOKEN_TYPE_STRING)
        eat_token(TOKEN_TYPE_STRING);
    else if (get_token()->type == TOKEN_TYPE_NUMBER)
        eat_token(TOKEN_TYPE_NUMBER);
    else if (get_token()->type == TOKEN_TYPE_BOOLEAN)
        eat_token(TOKEN_TYPE_BOOLEAN);
    else if (get_token()->type == TOKEN_TYPE_NULL)
        eat_token(TOKEN_TYPE_NULL);
}

static void parse_list()
{
    if (get_token()->type != TOKEN_TYPE_LEFT_BRACKET)
    {
        printf("Error: Invalid list : %s (l:%zu, c:%zu)\n", get_token()->value,
               get_token()->line, get_token()->column);
        return;
    }

    eat_token(TOKEN_TYPE_LEFT_BRACKET);

    if (is_value(get_token()))
    {
        parse_value();

        while (get_token()->type == TOKEN_TYPE_COMMA)
        {
            eat_token(TOKEN_TYPE_COMMA);
            parse_value();
        }
    }

    eat_token(TOKEN_TYPE_RIGHT_BRACKET);
}

static void parse_dict()
{
    if (get_token()->type != TOKEN_TYPE_LEFT_BRACE)
    {
        printf("Syntax error: Invalid dict : %s (l:%zu, c:%zu)\n",
               get_token()->value, get_token()->line, get_token()->column);
        return;
    }

    eat_token(TOKEN_TYPE_LEFT_BRACE);

    if (get_token()->type == TOKEN_TYPE_STRING)
    {
        parse_pair();

        while (get_token()->type == TOKEN_TYPE_COMMA)
        {
            eat_token(TOKEN_TYPE_COMMA);
            parse_pair();
        }
    }

    eat_token(TOKEN_TYPE_RIGHT_BRACE);
}

static void parse_pair()
{
    if (get_token()->type != TOKEN_TYPE_STRING)
    {
        printf("Syntax Error: Invalid key : %s (l:%zu, c:%zu)\n",
               get_token()->value, get_token()->line, get_token()->column);
        return;
    }

    eat_token(TOKEN_TYPE_STRING);
    eat_token(TOKEN_TYPE_COLON);

    parse_value();
}
