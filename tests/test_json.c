#include <criterion/criterion.h>

#include "criterion/assert.h"
#include "criterion/internal/assert.h"
#include "json.h"

Test(test_objects, test_str_attribute)
{
    char json_str[] = "{\"name\":\"John\",\"city\":\"New York\"}";

    struct json *json = json_parse(json_str);

    char *name = json_get_value(json, "name");
    cr_expect_str_eq(name, "John");

    cr_expect_str_eq(json_get_value(json, "name"), "John");
    cr_expect_str_eq(json_get_value(json, "city"), "New York");

    json_free(json);
}

Test(test_objects, test_backslashed_str_attribute)
{
    char json_str[] = "{\"name\":\"Im on a\nnew line that a \ttab\"}";

    struct json *json = json_parse(json_str);

    char *name = json_get_value(json, "name");
    cr_expect_str_eq(name, "Im on a\nnew line that a \ttab");

    json_free(json);
}

Test(test_objects, test_heavy_backslashed_str_attribute)
{
    char json_str[] = "{\"name\":\"Im on a\\nnew line\n\n\n\nthat a \\ttab\"}";

    struct json *json = json_parse(json_str);

    char *name = json_get_value(json, "name");
    cr_expect_str_eq(name, "Im on a\\nnew line\n\n\n\nthat a \\ttab");
}

Test(test_objects, test_int_attribute)
{
    char json_str[] = "{\"age\":30,\"height\":175}";

    struct json *json = json_parse(json_str);

    cr_expect_str_eq(json_get_value(json, "age"), "30");
    cr_expect_str_eq(json_get_value(json, "height"), "175");

    json_free(json);
}

Test(test_objects, test_float_attribute)
{
    char json_str[] = "{\"weight\":75.5,\"temperature\":36.6}";

    struct json *json = json_parse(json_str);

    cr_expect_str_eq(json_get_value(json, "weight"), "75.5");
    cr_expect_str_eq(json_get_value(json, "temperature"), "36.6");

    json_free(json);
}

Test(test_objects, test_bool_attribute)
{
    char json_str[] = "{\"is_human\":true,\"is_cool\":false}";

    struct json *json = json_parse(json_str);

    cr_expect_str_eq(json_get_value(json, "is_human"), "1");
    cr_expect_str_eq(json_get_value(json, "is_cool"), "0");
}

Test(test_objects, test_null_attribute)
{
    char json_str[] = "{\"is_null\":null}";

    struct json *json = json_parse(json_str);

    cr_expect_eq(json_get_value(json, "is_null"), NULL);

    json_free(json);
}

Test(test_nested_objects, test_nested_objects_simple)
{
    char json_str[] = "{\"person\":{\"name\":\"John\",\"age\":30}}";

    struct json *json = json_parse(json_str);

    cr_expect_str_eq(json_get_value(json, "person.name"), "John");
    cr_expect_str_eq(json_get_value(json, "person.age"), "30");

    json_free(json);
}

Test(test_nested_objects, test_nested_objects_double)
{
    char json_str[] = "{\"person\":{\"name\":\"John\",\"age\":30},\"address\":{"
                      "\"city\":\"New York\",\"country\":\"USA\"}}";

    struct json *json = json_parse(json_str);

    cr_expect_str_eq(json_get_value(json, "person.name"), "John");
    cr_expect_str_eq(json_get_value(json, "person.age"), "30");
    cr_expect_str_eq(json_get_value(json, "address.city"), "New York");
    cr_expect_str_eq(json_get_value(json, "address.country"), "USA");

    json_free(json);
}

Test(test_nested_objects, test_nested_objects_deep)
{
    char json_str[] = "{\"person\":{\"name\":\"John\",\"age\":30,\"address\":{"
                      "\"city\":\"New York\",\"country\":\"USA\"}}}";

    struct json *json = json_parse(json_str);

    cr_expect_str_eq(json_get_value(json, "person.name"), "John");
    cr_expect_str_eq(json_get_value(json, "person.age"), "30");
    cr_expect_str_eq(json_get_value(json, "person.address.city"), "New York");
    cr_expect_str_eq(json_get_value(json, "person.address.country"), "USA");

    json_free(json);
}

Test(test_arrays, test_array_attribute)
{
    char json_str[] = "{\"numbers\":[1,2,3,4,5,6,7,8,9,10,11]}";

    struct json *json = json_parse(json_str);

    cr_expect_str_eq(json_get_value(json, "numbers[0]"), "1");
    cr_expect_str_eq(json_get_value(json, "numbers[1]"), "2");
    cr_expect_str_eq(json_get_value(json, "numbers[10]"), "11");

    json_free(json);
}

Test(test_arrays, test_object_in_array_attribute)
{
    char json_str[] = "{\"numbers\":[{\"one\":1},{\"two\":2},{\"three\":3}]}";

    struct json *json = json_parse(json_str);

    cr_expect_str_eq(json_get_value(json, "numbers[0].one"), "1");
    cr_expect_str_eq(json_get_value(json, "numbers[1].two"), "2");
    cr_expect_str_eq(json_get_value(json, "numbers[2].three"), "3");

    json_free(json);
}

Test(test_arrays, test_array_in_array_attribute)
{
    char json_str[] = "{\"numbers\":[[1,2,3],[4,5,6],[7,8,9]]}";

    struct json *json = json_parse(json_str);

    cr_expect_str_eq(json_get_value(json, "numbers[0][0]"), "1");
    cr_expect_str_eq(json_get_value(json, "numbers[0][1]"), "2");
    cr_expect_str_eq(json_get_value(json, "numbers[0][2]"), "3");
    cr_expect_str_eq(json_get_value(json, "numbers[1][0]"), "4");
    cr_expect_str_eq(json_get_value(json, "numbers[1][1]"), "5");
    cr_expect_str_eq(json_get_value(json, "numbers[1][2]"), "6");
    cr_expect_str_eq(json_get_value(json, "numbers[2][0]"), "7");
    cr_expect_str_eq(json_get_value(json, "numbers[2][1]"), "8");
    cr_expect_str_eq(json_get_value(json, "numbers[2][2]"), "9");

    json_free(json);
}

Test(test_arrays, test_array_in_array_in_array_attribute)
{
    char json_str[] = "{\"numbers\":[[[1,2,3],[4,5,6],[7,8,9]]]}";

    struct json *json = json_parse(json_str);

    cr_expect_str_eq(json_get_value(json, "numbers[0][0][0]"), "1");
    cr_expect_str_eq(json_get_value(json, "numbers[0][0][1]"), "2");
    cr_expect_str_eq(json_get_value(json, "numbers[0][0][2]"), "3");
    cr_expect_str_eq(json_get_value(json, "numbers[0][1][0]"), "4");
    cr_expect_str_eq(json_get_value(json, "numbers[0][1][1]"), "5");
    cr_expect_str_eq(json_get_value(json, "numbers[0][1][2]"), "6");
    cr_expect_str_eq(json_get_value(json, "numbers[0][2][0]"), "7");
    cr_expect_str_eq(json_get_value(json, "numbers[0][2][1]"), "8");
    cr_expect_str_eq(json_get_value(json, "numbers[0][2][2]"), "9");

    json_free(json);
}

Test(test_top_level, top_level_null)
{
    char json_str[] = "null";

    struct json *json = json_parse(json_str);

    cr_expect_eq(json_get_value(json, ""), NULL);
    cr_expect_eq(json_get_value(json, NULL), NULL);

    json_free(json);
}

Test(test_top_level, top_level_true)
{
    char json_str[] = "true";

    struct json *json = json_parse(json_str);

    cr_expect_str_eq(json_get_value(json, ""), "1");
    cr_expect_str_eq(json_get_value(json, NULL), "1");

    json_free(json);
}

Test(test_top_level, top_level_false)
{
    char json_str[] = "false";

    struct json *json = json_parse(json_str);

    cr_expect_str_eq(json_get_value(json, ""), "0");
    cr_expect_str_eq(json_get_value(json, NULL), "0");

    json_free(json);
}

Test(test_top_level, top_level_string)
{
    char json_str[] = "\"Hello World\"";

    struct json *json = json_parse(json_str);

    cr_expect_str_eq(json_get_value(json, ""), "Hello World");
    cr_expect_str_eq(json_get_value(json, NULL), "Hello World");

    json_free(json);
}

Test(test_top_level, top_level_number)
{
    char json_str[] = "12345";

    struct json *json = json_parse(json_str);

    cr_expect_str_eq(json_get_value(json, ""), "12345");
    cr_expect_str_eq(json_get_value(json, NULL), "12345");

    json_free(json);
}

Test(test_top_level, top_level_array)
{
    char json_str[] = "[1,2,3,4,5,6,7,8,9,10,11]";

    struct json *json = json_parse(json_str);

    cr_expect_str_eq(json_get_value(json, "[0]"), "1");
    cr_expect_str_eq(json_get_value(json, "[1]"), "2");
    cr_expect_str_eq(json_get_value(json, "[10]"), "11");

    json_free(json);
}

Test(test_top_level, top_level_arrayin_array)
{
    char json_str[] = "[[1,2,3],[4,5,6],[7,8,9]]";

    struct json *json = json_parse(json_str);

    cr_expect_str_eq(json_get_value(json, "[0][0]"), "1");
    cr_expect_str_eq(json_get_value(json, "[0][1]"), "2");
    cr_expect_str_eq(json_get_value(json, "[0][2]"), "3");
    cr_expect_str_eq(json_get_value(json, "[1][0]"), "4");
    cr_expect_str_eq(json_get_value(json, "[1][1]"), "5");
    cr_expect_str_eq(json_get_value(json, "[1][2]"), "6");
    cr_expect_str_eq(json_get_value(json, "[2][0]"), "7");
    cr_expect_str_eq(json_get_value(json, "[2][1]"), "8");
    cr_expect_str_eq(json_get_value(json, "[2][2]"), "9");

    json_free(json);
}

Test(test_top_level, top_level_object)
{
    char json_str[] = "{\"one\":1,\"two\":2,\"three\":3}";

    struct json *json = json_parse(json_str);

    cr_expect_str_eq(json_get_value(json, "one"), "1");
    cr_expect_str_eq(json_get_value(json, "two"), "2");
    cr_expect_str_eq(json_get_value(json, "three"), "3");

    json_free(json);
}

Test(test_getters, test_get_bool)
{
    char json_str[] = "{\"bool\":true}";

    struct json *json = json_parse(json_str);

    cr_expect_eq(json_get_bool(json, "bool"), 1);

    json_free(json);
}

Test(test_getters, test_get_int)
{
    char json_str[] = "{\"int\":12345}";

    struct json *json = json_parse(json_str);

    cr_expect_eq(json_get_int(json, "int"), 12345);

    json_free(json);
}

Test(test_getters, test_get_double)
{
    char json_str[] = "{\"double\":12345.6789}";

    struct json *json = json_parse(json_str);

    cr_expect_eq(json_get_double(json, "double"), 12345.6789);

    json_free(json);
}

Test(test_getters, test_get_float)
{
    char json_str[] = "{\"float\":12345.6789}";

    struct json *json = json_parse(json_str);

    cr_expect_leq(json_get_float(json, "float") - 1, 12345.6789);

    json_free(json);
}

Test(test_bad_query, bad_query_object)
{
    char json_str[] = "{\"one\":1,\"two\":2,\"three\":3}";

    struct json *json = json_parse(json_str);

    cr_expect_eq(json_get_value(json, "one.two"), NULL);

    json_free(json);
}

Test(test_bad_query, bad_query_array)
{
    char json_str[] = "[1,2,3,4,5,6,7,8,9,10,11]";

    struct json *json = json_parse(json_str);

    cr_expect_eq(json_get_value(json, "[0][1]"), NULL);

    json_free(json);
}

Test(test_bad_query, bad_query_array_object)
{
    char json_str[] = "[{\"one\":1},{\"two\":2},{\"three\":3}]";

    struct json *json = json_parse(json_str);

    cr_expect_eq(json_get_value(json, "[0].one.two"), NULL);

    json_free(json);
}

Test(test_bad_query, bad_query_array_array)
{
    char json_str[] = "[[1,2,3],[4,5,6],[7,8,9]]";

    struct json *json = json_parse(json_str);

    cr_expect_eq(json_get_value(json, "[0][0][1]"), NULL);

    json_free(json);
}

Test(test_bad_query, malformed_query)
{
    char json_str[] = "[[1,2,3],[4,5,6],[7,8,9]]";

    struct json *json = json_parse(json_str);

    cr_expect_eq(json_get_int(json, "[[dw]][[]dwa]"), 1);

    json_free(json);
}

Test(test_bad_query, malformed_query_injection)
{
    char json_str[] = "[[1,2,3],[4,5,6],[7,8,9]]";

    struct json *json = json_parse(json_str);

    cr_expect_eq(json_get_value(json, "arr[[dw]][[]dwa]"), NULL);

    json_free(json);
}
