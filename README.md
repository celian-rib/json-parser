# JSON Parser
> Lexer + Parser LL(1) + Solver

> Made for C

### Philosophy

- Ease of use
- Fast
- Robust

### How to use

```c
// Parse your input string into a json object
struct json_object *json = parse_json(input);

// Get values from the json object using a query string
char* = get_value(json, "value1.value2.value4");
```

> If the query string **does not match** anything `get_value` will return `NULL`.

### Examples

#### 1/ Attribute from nested objects

```json
{
    "user": {
        "name": "Michel",
        "age": 87,
    }
}
```

> You can get the age from `user` using:

```c
int age = get_value(json, "user.age");
```

---

#### 2/ Array element

```json
{
    "my_array": ["tab1", "tab2"] 
}
```

> You can get `tab1` using:

```c
char *tab1 = get_value(json, "my_array[0]");
```

---

#### 3/ Attribute from object within array

```json
{
    "users": [
       {
            "name": "Michel",
            "age": 87,
       },
       {
            "name": "Jo",
            "age": 86,
       } 
    ] 
}
```

> You can get `Jo` using:

```c
char *name = get_value(json, "users[1].name");
```

---

#### 4/ Top level double array element

```json
[
    1,
    2,
    [3, 4]
]
```

> You can get `4` using:

```c
int val = get_value(json, "[2][1]");
```

#### 5/ Top level value

```json
"Tequilla"
```

> You can get `Tequilla` using:

```c
int val = get_value(json, "");
// or
int val = get_value(json, NULL);
```

#### 6/ Object

```json
{
    "my_obj": {
        "key": "value"
    }
}
```

> You can get `my_obj` using:

```c
struct json_object *my_obj = get_value(json, "my_obj");
```

### Exended grammar

```
json -> value $
value -> dict | list | STRING | NUMBER | "true" | "false" | "null"
list -> "[" value ("," value)* "]"
dict -> "{" pair ("," pair)* "}"
pair -> STRING ":" value
```

