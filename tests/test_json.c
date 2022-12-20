#include <criterion/criterion.h>

#include "json_object.h"
#include "parser.h"

Test(test_objects, test_str_attribute)
{
    char json_str[] = "{\"name\":\"John\",\"city\":\"New York\"}";

    struct json_object *json = parse_json(json_str);

    char *name = get_value(json, "name");
    cr_expect_str_eq(name, "John");

    cr_expect_str_eq(get_value(json, "name"), "John");
    cr_expect_str_eq(get_value(json, "city"), "New York");

    free_json_object(json);
}

Test(test_objects, test_int_attribute)
{
    char json_str[] = "{\"age\":30,\"height\":175}";

    struct json_object *json = parse_json(json_str);

    cr_expect_str_eq(get_value(json, "age"), "30");
    cr_expect_str_eq(get_value(json, "height"), "175");

    free_json_object(json);
}

Test(test_objects, test_float_attribute)
{
    char json_str[] = "{\"weight\":75.5,\"temperature\":36.6}";

    struct json_object *json = parse_json(json_str);

    cr_expect_str_eq(get_value(json, "weight"), "75.5");
    cr_expect_str_eq(get_value(json, "temperature"), "36.6");

    free_json_object(json);
}

Test(test_objects, test_bool_attribute)
{
    char json_str[] = "{\"is_human\":true,\"is_cool\":false}";

    struct json_object *json = parse_json(json_str);

    cr_expect_str_eq(get_value(json, "is_human"), "1");
    cr_expect_str_eq(get_value(json, "is_cool"), "0");
}

Test(test_objects, test_null_attribute)
{
    char json_str[] = "{\"is_null\":null}";

    struct json_object *json = parse_json(json_str);

    cr_expect_eq(*get_value(json, "is_null"), '\0');

    free_json_object(json);
}

Test(test_nested_objects, test_nested_objects_simple)
{
    char json_str[] = "{\"person\":{\"name\":\"John\",\"age\":30}}";

    struct json_object *json = parse_json(json_str);

    cr_expect_str_eq(get_value(json, "person.name"), "John");
    cr_expect_str_eq(get_value(json, "person.age"), "30");

    free_json_object(json);
}

Test(test_nested_objects, test_nested_objects_double)
{
    char json_str[] = "{\"person\":{\"name\":\"John\",\"age\":30},\"address\":{"
                      "\"city\":\"New York\",\"country\":\"USA\"}}";

    struct json_object *json = parse_json(json_str);

    cr_expect_str_eq(get_value(json, "person.name"), "John");
    cr_expect_str_eq(get_value(json, "person.age"), "30");
    cr_expect_str_eq(get_value(json, "address.city"), "New York");
    cr_expect_str_eq(get_value(json, "address.country"), "USA");

    free_json_object(json);
}

Test(test_nested_objects, test_nested_objects_deep)
{
    char json_str[] = "{\"person\":{\"name\":\"John\",\"age\":30,\"address\":{"
                      "\"city\":\"New York\",\"country\":\"USA\"}}}";

    struct json_object *json = parse_json(json_str);

    cr_expect_str_eq(get_value(json, "person.name"), "John");
    cr_expect_str_eq(get_value(json, "person.age"), "30");
    cr_expect_str_eq(get_value(json, "person.address.city"), "New York");
    cr_expect_str_eq(get_value(json, "person.address.country"), "USA");

    free_json_object(json);
}

// Test(test_objects, test_array_attribute)
// {
//     char json_str[] = "{\"numbers\":[1,2,3,4,5,6,7,8,9,10,11]}";
//
//     struct json_object *json = parse_json(json_str);
//
//     cr_expect_str_eq(get_value(json, "numbers[0]"), "1");
//     cr_expect_str_eq(get_value(json, "numbers[1]"), "2");
//     cr_expect_str_eq(get_value(json, "numbers[10]"), "11");
//
//     free_json_object(json);
// }
