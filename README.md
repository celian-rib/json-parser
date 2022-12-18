
```
json -> value $
value -> dict | list | STRING | NUMBER | "true" | "false" | "null"
list -> "[" value ("," value)* "]"
dict -> "{" pair ("," pair)* "}"
pair -> STRING ":" value
```

