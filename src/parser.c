#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "json.h"
#include "lexer.h"

static struct json *parse_value();
static struct json *parse_list();
static struct json *parse_dict();
static struct json *parse_pair();

static struct json *create_node(struct token *token)
{
    struct json *node = malloc(sizeof(struct json));
    node->token = token;
    node->children = NULL;
    node->children_count = 0;
    return node;
}

static void node_add_child(struct json *node, struct json *child)
{
    node->children = realloc(
        node->children, (node->children_count + 1) * sizeof(struct json *));
    node->children[node->children_count++] = child;
}

static bool is_value(struct token *token)
{
    return token->type == TOKEN_TYPE_NUMBER //
        || token->type == TOKEN_TYPE_STRING //
        || token->type == TOKEN_TYPE_NULL //
        || token->type == TOKEN_TYPE_BOOLEAN;
}

struct json *json_parse(char *json)
{
    lexer_init(json);

    struct json *node = create_node(get_token());

    if (is_value(get_token()))
    {
        node = parse_value();
    }
    else if (get_token()->type == TOKEN_TYPE_LEFT_BRACE)
    {
        node = parse_dict();
    }
    else if (get_token()->type == TOKEN_TYPE_LEFT_BRACKET)
    {
        node = parse_list();
    }

    eat_token(TOKEN_TYPE_EOF);
    return node;
}

static struct json *parse_value()
{
    if (get_token()->type == TOKEN_TYPE_LEFT_BRACE)
    {
        return parse_dict();
    }
    else if (get_token()->type == TOKEN_TYPE_LEFT_BRACKET)
    {
        return parse_list();
    }
    else if (get_token()->type == TOKEN_TYPE_STRING)
    {
        struct json *node = create_node(get_token());
        eat_token(TOKEN_TYPE_STRING);
        return node;
    }
    else if (get_token()->type == TOKEN_TYPE_NUMBER)
    {
        struct json *node = create_node(get_token());
        eat_token(TOKEN_TYPE_NUMBER);
        return node;
    }
    else if (get_token()->type == TOKEN_TYPE_BOOLEAN)
    {
        struct json *node = create_node(get_token());
        eat_token(TOKEN_TYPE_BOOLEAN);
        return node;
    }
    else if (get_token()->type == TOKEN_TYPE_NULL)
    {
        struct json *node = create_node(get_token());
        eat_token(TOKEN_TYPE_NULL);
        return node;
    }

    return NULL;
}

static struct json *parse_list()
{
    struct json *node = create_node(get_token());
    eat_token(TOKEN_TYPE_LEFT_BRACKET);

    struct json *child = parse_value();
    node_add_child(node, child);

    while (get_token()->type == TOKEN_TYPE_COMMA)
    {
        eat_token(TOKEN_TYPE_COMMA);
        child = parse_value();
        node_add_child(node, child);
    }

    eat_token(TOKEN_TYPE_RIGHT_BRACKET);
    return node;
}

static struct json *parse_dict()
{
    struct json *node = create_node(get_token());
    eat_token(TOKEN_TYPE_LEFT_BRACE);

    struct json *child = parse_pair();
    node_add_child(node, child);

    while (get_token()->type == TOKEN_TYPE_COMMA)
    {
        eat_token(TOKEN_TYPE_COMMA);
        child = parse_pair();
        node_add_child(node, child);
    }

    eat_token(TOKEN_TYPE_RIGHT_BRACE);
    return node;
}

static struct json *parse_pair()
{
    struct json *node = create_node(get_token());

    eat_token(TOKEN_TYPE_STRING);
    eat_token(TOKEN_TYPE_COLON);

    struct json *child = parse_value();
    node_add_child(node, child);

    return node;
}
