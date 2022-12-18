#include "parser.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../lexer/lexer.h"
#include "../object/json_object.h"

static struct json_object *parse_value();
static struct json_object *parse_list();
static struct json_object *parse_dict();
static struct json_object *parse_pair();

static struct json_object *create_node(struct token *token)
{
    struct json_object *node = malloc(sizeof(struct json_object));
    node->token = token;
    node->children = NULL;
    node->children_count = 0;
    return node;
}

static void node_add_child(struct json_object *node, struct json_object *child)
{
    node->children =
        realloc(node->children,
                (node->children_count + 1) * sizeof(struct json_object *));
    node->children[node->children_count++] = child;
}

static bool is_value(struct token *token)
{
    return token->type == TOKEN_TYPE_NUMBER //
        || token->type == TOKEN_TYPE_STRING //
        || token->type == TOKEN_TYPE_NULL //
        || token->type == TOKEN_TYPE_BOOLEAN;
}

struct json_object *parse_json(char *json)
{
    lexer_init(json);

    struct json_object *node = create_node(get_token());

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

static struct json_object *parse_value()
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
        struct json_object *node = create_node(get_token());
        eat_token(TOKEN_TYPE_STRING);
        return node;
    }
    else if (get_token()->type == TOKEN_TYPE_NUMBER)
    {
        struct json_object *node = create_node(get_token());
        eat_token(TOKEN_TYPE_NUMBER);
        return node;
    }
    else if (get_token()->type == TOKEN_TYPE_BOOLEAN)
    {
        struct json_object *node = create_node(get_token());
        eat_token(TOKEN_TYPE_BOOLEAN);
        return node;
    }
    else if (get_token()->type == TOKEN_TYPE_NULL)
    {
        struct json_object *node = create_node(get_token());
        eat_token(TOKEN_TYPE_NULL);
        return node;
    }

    return NULL;
}

static struct json_object *parse_list()
{
    struct json_object *node = create_node(get_token());
    eat_token(TOKEN_TYPE_LEFT_BRACKET);

    struct json_object *child = parse_value();
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

static struct json_object *parse_dict()
{
    struct json_object *node = create_node(get_token());
    eat_token(TOKEN_TYPE_LEFT_BRACE);

    struct json_object *child = parse_pair();
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

static struct json_object *parse_pair()
{
    struct json_object *node = create_node(get_token());

    eat_token(TOKEN_TYPE_STRING);
    eat_token(TOKEN_TYPE_COLON);

    struct json_object *child = parse_value();
    node_add_child(node, child);

    return node;
}
