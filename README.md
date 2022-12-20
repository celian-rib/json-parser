# JSON Parser
> Lexer + Parser LL(1) + Solver

> Made for C

### How to use

```c
// Parse your input string into a json object
json_object *json = json_parse(input);

// Get values from the json object using a query string
char* = json_get_value(json, "value1.value2.value4");

// Clear everything when done
json_free(json);
```

> If the query string **does not match** anything `get_value` will return `NULL`.

### API references

```c
json *json_parse(char *json);

json *json_parse_file(FILE *file);

char *json_get_value(struct json *object, char *key);

bool json_get_bool(struct json *object, char *key);

int json_get_int(struct json *object, char *key);

double json_get_double(struct json *object, char *key);

float json_get_float(struct json *object, char *key);

void json_free(struct json *object);
```

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
int age = json_get_int(json, "user.age");
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
int val = json_get_int(json, "[2][1]");
```

#### 5/ Top level value

```json
"Tequilla"
```

> You can get `Tequilla` using:

```c
char *val = json_get_value(json, "");
// or
char *val = json_get_value(json, NULL);
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
json_object *my_obj = get_value(json, "my_obj");
```

### Exended grammar

```
json -> value $
value -> dict | list | STRING | NUMBER | "true" | "false" | "null"
list -> "[" value ("," value)* "]"
dict -> "{" pair ("," pair)* "}"
pair -> STRING ":" value
```

