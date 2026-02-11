Library AST FRI for C\#
===

- Supported C# versions by Tree-Sitter C# grammar: C# 1 - C# 13.0
- TODO subproject description

Build
---

- TODO

For Developers
---

### Variable naming prefixes and suffixes

#### Prefixes

- `n` - variable of `TSNode` type
- `h` - variable of `Handler` type
- `q` - query related variable (could be `TSQuery`, `Query`, string representation of Tree-Sitter query or type of
  query - `QueryType`)
- `c` - count variable
- `s` - variable of `TSymbol` type
- `nms` - namespace related variable

#### Suffixes

- `Fact` - factory variable
- `Meta` - variable of Metadata type
- `Sw` - varaible `string_view` type