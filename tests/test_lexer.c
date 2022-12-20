#include <criterion/criterion.h>

#include "criterion/assert.h"
#include "lexer.h"

#define EXPECT_INT_EQ(value, expected)                                         \
    cr_expect_eq(value, expected, "\nExpected '%d', \nGot      '%zu'",         \
                 expected, value);

Test(test_lexer, test_get_string)
{
    char json_str[] = "\"hello world\"";

    lexer_init(json_str);

    struct token *token = get_token();
    cr_expect_eq(get_token_type(), TOKEN_TYPE_STRING);
    cr_expect_str_eq(token->value, "hello world");
    EXPECT_INT_EQ(token->line, 1);
    EXPECT_INT_EQ(token->column, 1);

    free_token(token);
}

Test(test_lexer, test_get_escaped_string)
{
    char json_str[] = "\"hello \\\"world\\\"\"";

    lexer_init(json_str);

    struct token *token = get_token();
    cr_expect_eq(get_token_type(), TOKEN_TYPE_STRING);
    cr_expect_str_eq(token->value, "hello \\\"world\\\"");
    EXPECT_INT_EQ(token->line, 1);
    EXPECT_INT_EQ(token->column, 1);

    free_token(token);
}

Test(test_lexer, test_get_line_feed_string)
{
    char json_str[] = "\"hello \\nworld\"";

    lexer_init(json_str);

    struct token *token = get_token();
    cr_expect_eq(get_token_type(), TOKEN_TYPE_STRING);
    cr_expect_str_eq(token->value, "hello \\nworld");
    EXPECT_INT_EQ(token->line, 1);
    EXPECT_INT_EQ(token->column, 1);

    free_token(token);
}

Test(test_lexer, test_get_number)
{
    char json_str[] = "123";

    lexer_init(json_str);

    struct token *token = get_token();
    cr_expect_eq(get_token_type(), TOKEN_TYPE_NUMBER);
    cr_expect_str_eq(token->value, "123");
    EXPECT_INT_EQ(token->line, 1);
    EXPECT_INT_EQ(token->column, 1);

    free_token(token);
}

Test(test_lexer, test_get_negative_number)
{
    char json_str[] = "-123";

    lexer_init(json_str);

    struct token *token = get_token();
    cr_expect_eq(get_token_type(), TOKEN_TYPE_NUMBER);
    cr_expect_str_eq(token->value, "-123");
    EXPECT_INT_EQ(token->line, 1);
    EXPECT_INT_EQ(token->column, 1);

    free_token(token);
}

Test(test_lexer, test_get_float_number)
{
    char json_str[] = "123.456";

    lexer_init(json_str);

    struct token *token = get_token();
    cr_expect_eq(get_token_type(), TOKEN_TYPE_NUMBER);
    cr_expect_str_eq(token->value, "123.456");
    EXPECT_INT_EQ(token->line, 1);
    EXPECT_INT_EQ(token->column, 1);

    free_token(token);
}

Test(test_lexer, test_get_negative_float_number)
{
    char json_str[] = "-123.456";

    lexer_init(json_str);

    struct token *token = get_token();
    cr_expect_eq(get_token_type(), TOKEN_TYPE_NUMBER);
    cr_expect_str_eq(token->value, "-123.456");
    EXPECT_INT_EQ(token->line, 1);
    EXPECT_INT_EQ(token->column, 1);

    free_token(token);
}

Test(test_lexer, test_true)
{
    char json_str[] = "true";

    lexer_init(json_str);

    struct token *token = get_token();
    cr_expect_eq(get_token_type(), TOKEN_TYPE_BOOLEAN);
    cr_expect_str_eq(token->value, "1");
    EXPECT_INT_EQ(token->line, 1);
    EXPECT_INT_EQ(token->column, 1);

    free_token(token);
}

Test(test_lexer, test_false)
{
    char json_str[] = "false";

    lexer_init(json_str);

    struct token *token = get_token();
    cr_expect_eq(get_token_type(), TOKEN_TYPE_BOOLEAN);
    cr_expect_str_eq(token->value, "0");
    EXPECT_INT_EQ(token->line, 1);
    EXPECT_INT_EQ(token->column, 1);

    free_token(token);
}

Test(test_lexer, test_null)
{
    char json_str[] = "null";

    lexer_init(json_str);

    struct token *token = get_token();
    cr_expect_eq(get_token_type(), TOKEN_TYPE_NULL);
    cr_expect_eq(token->value, NULL);
    EXPECT_INT_EQ(token->line, 1);
    EXPECT_INT_EQ(token->column, 1);

    free_token(token);
}

Test(test_lexer, test_brace)
{
    char json_str[] = "{}";

    lexer_init(json_str);

    struct token *token = get_token();
    cr_expect_eq(get_token_type(), TOKEN_TYPE_LEFT_BRACE);
    cr_expect_eq(*token->value, '{');
    EXPECT_INT_EQ(token->line, 1);
    EXPECT_INT_EQ(token->column, 1);

    eat_token(TOKEN_TYPE_LEFT_BRACE);
    free_token(token);

    token = get_token();
    cr_expect_eq(get_token_type(), TOKEN_TYPE_RIGHT_BRACE);
    cr_expect_eq(*token->value, '}');
    EXPECT_INT_EQ(token->line, 1);
    EXPECT_INT_EQ(token->column, 2);

    eat_token(TOKEN_TYPE_RIGHT_BRACE);
    free_token(token);
}

Test(test_lexer, test_bracket)
{
    char json_str[] = "[  ]";

    lexer_init(json_str);

    struct token *token = get_token();
    cr_expect_eq(get_token_type(), TOKEN_TYPE_LEFT_BRACKET);
    cr_expect_eq(*token->value, '[');
    EXPECT_INT_EQ(token->line, 1);
    EXPECT_INT_EQ(token->column, 1);

    eat_token(TOKEN_TYPE_LEFT_BRACKET);
    free_token(token);

    token = get_token();
    cr_expect_eq(get_token_type(), TOKEN_TYPE_RIGHT_BRACKET);
    cr_expect_eq(*token->value, ']');
    EXPECT_INT_EQ(token->line, 1);
    EXPECT_INT_EQ(token->column, 4);

    eat_token(TOKEN_TYPE_RIGHT_BRACKET);
    free_token(token);
}

Test(test_lexer, test_get_token)
{
    char json_str[] = "{\"name\":\"John\"  }";

    lexer_init(json_str);

    struct token *token = get_token();
    cr_expect_eq(get_token_type(), TOKEN_TYPE_LEFT_BRACE);
    cr_expect_eq(*token->value, '{');
    EXPECT_INT_EQ(token->line, 1);
    EXPECT_INT_EQ(token->column, 1);

    eat_token(TOKEN_TYPE_LEFT_BRACE);
    free_token(token);

    token = get_token();
    cr_expect_eq(get_token_type(), TOKEN_TYPE_STRING);
    cr_expect_str_eq(token->value, "name");
    EXPECT_INT_EQ(token->line, 1);
    EXPECT_INT_EQ(token->column, 2);

    eat_token(TOKEN_TYPE_STRING);
    free_token(token);

    token = get_token();
    cr_expect_eq(get_token_type(), TOKEN_TYPE_COLON);
    cr_expect_str_eq(token->value, ":");
    EXPECT_INT_EQ(token->line, 1);
    EXPECT_INT_EQ(token->column, 8);

    eat_token(TOKEN_TYPE_COLON);
    free_token(token);

    token = get_token();
    cr_expect_eq(get_token_type(), TOKEN_TYPE_STRING);
    cr_expect_str_eq(token->value, "John");
    EXPECT_INT_EQ(token->line, 1);
    EXPECT_INT_EQ(token->column, 9);

    eat_token(TOKEN_TYPE_STRING);
    free_token(token);

    token = get_token();
    cr_expect_eq(get_token_type(), TOKEN_TYPE_RIGHT_BRACE);
    cr_expect_str_eq(token->value, "}");
    EXPECT_INT_EQ(token->line, 1);
    EXPECT_INT_EQ(token->column, 17);
}
