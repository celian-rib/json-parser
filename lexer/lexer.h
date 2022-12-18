#include <stddef.h>
#ifndef LEXER_H

enum TOKEN_TYPE
{
    // No token
    TOKEN_TYPE_NONE = 'x',

    // File end
    TOKEN_TYPE_EOF = 'e',

    // Content
    TOKEN_TYPE_BOOLEAN = 'b',
    TOKEN_TYPE_NUMBER = 'n',
    TOKEN_TYPE_STRING = 's',
    TOKEN_TYPE_NULL = 'l',

    // Separators
    TOKEN_TYPE_COMMA = ',',
    TOKEN_TYPE_COLON = ':',

    TOKEN_TYPE_LEFT_BRACE = '{',
    TOKEN_TYPE_RIGHT_BRACE = '}',

    TOKEN_TYPE_LEFT_BRACKET = '[',
    TOKEN_TYPE_RIGHT_BRACKET = ']'
};

struct token
{
    enum TOKEN_TYPE type;
    char *value;
    size_t line;
    size_t column;
};

void lexer_init(char *json);

/*
 * Consume current token and returns it
 */
struct token *eat_token(enum TOKEN_TYPE expected_type);

/*
 * Get the next token without consuming it.
 */
struct token *get_token();

/*
 * Get the next token type without consuming it
 */
enum TOKEN_TYPE get_token_type();

#endif // !LEXER_H
