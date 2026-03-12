Library AST FRI for C\#
===

- Supported C# versions by Tree-Sitter C# grammar: C# 1 - C# 13.0
- TODO subproject description

Library Usage
---

### Using in code

1. Link to library in `CMakeLists.txt` in your project
    ```cmake
   target_link_libraries(astfri-cs-examples PRIVATE astfri-cs)
   ```
2. Include these header:
    ```c++
    #include <libastfri-cs/inc/ASTBuilder.hpp>
    #include <libastfri-text/inc/TextLibManager.hpp>
    #include <libastfri/inc/Astfri.hpp>
   ```
3. Create instance of `ASTBuilder` (inside `astfri::csharp` namespace)
4. Call method `load_src` to load a source code:
    1. using a path to project's root directory (root directory is the directory with `.csproj` file). If project has
       subprojects load each one separatly.
    ```c++
    astfri::csharp::ASTBuilder;
    astBuilder.load_src("/absolute/path/to/projects/root/directory");
   ```
    2. using a inputstream
    ```c++
    astfri::csharp::ASTBuilder;
    std::istream inputStream;
    astBuilder.load_src(inputStream);
   ```
5. After loading every file/subproject into ASTBuilder, call `mk_ast` method
    ```c++
    TranslationUnit* ast = astBuilder.mk_ast();
    ```

### Input Requirements & Preconditions

- Library expects input code to be compilable by C# compiler. That means code should't contain:
    - multiple variables having same name (except when shadowing member variables)
    - multiple types having same name in the same scope (or any other C# naming rule violation)
    - nested class having same name as outer class
    - multiple aliases with the same name
- If code contains any errors that Tree-sitter can't detect (like those mentioned above) library might give invalid
  output

Build
---

- TODO

Supported language features
---

[//]: # (✅, ⚠️, ❌)

### Directives & Dependencies

- ❌ external aliases
- ✅ global using directives
- ✅ global aliases
- ✅ global static using directives
- ✅ file scoped using directives
- ⚠️ ️local static using directives - loading supported
- ⚠️ local aliases - loading supported
- ✅ namespace scoping

### Type Declaration

#### Standard Types

- ✅ integer
- ✅ floating point
- ✅ character
- ✅ string
- `var` - deduced type
- `_` - discard variable
- ❌ tuple type
    - ❌ unnamed tuple
- ❌ array type
- ✅ pointer type
- ✅ reference type
    - ✅ out - mapped to ref
    - ⚠ in - mapped to readonly ref
    - ✅ ref
    - ⚠️ readonly ref - for now mapped to ref
- ⚠️ nullable
- ❌ generic type
- ❌ function pointer type
- ❌ scoped type

#### User Defined Types

- ✅ class
    - ✅ static class - mapped to normal class
    - ✅ inheritance
    - ✅ interface implementation
    - ✅ member variables
        - ✅ instance member variables
        - ⚠️ static variables - for now mapped to instance member variables
    - ❌ properties
        - ❌ auto properties
        - ❌ explicit properties - with defined body
        - ❌ static properties
        - ❌ abstract
        - ❌ virtual
    - ✅ methods
        - ✅ instance methods
        - ⚠️ static methods - for now mapped to instance methods
    - ❌ indexer
        - ❌ instance
        - ❌ static
    - ✅ constructors
        - ✅ base initializer
        - ✅ self initialized
        - ❌ primary constructor
    - ✅ destructors
    - ❌ operator overloading
    - ❌ cast overloading
        - ❌ explicit
        - ❌ implicit
    - ❌ event field
- ✅ struct - mapped to class
- ✅ interface
    - ✅ interface inheritance
    - ❌ member variables
        - ❌ static variables
    - ❌ properties
        - ❌ auto properties
        - ❌ static properties
    - ✅ methods
        - ✅ instance methods
        - ⚠️ static methods - for now mapped to instance methods
    - ❌ indexer
        - ❌ instance
        - ❌ static
    - ❌ operator overloading
    - ❌ cast overloading
        - ❌ explicit
        - ❌ implicit
    - ❌ event field
- ❌ record/struct record
- ❌ enum
- ❌ delegate
- ❌ access modifiers for types - public, internal, file (private, protected for nested types)
- ⚠️ nested types - supported but access modifiers are not so it might produce incorrect output
- ❌ generics
- ❌ extention methods
- ⚠️ variadic parameters (`params`) - partialy implemented; in output, variadic parameters will be mapped to its
  underlying type
- ✅ partial classes
    - ❌ partial members
- ❌ anonymous type

### Statements

- ✅ variable declaration
    - ✅ parameter
    - ✅ local variable
- ✅ expression statement
- ✅ body
    - ✅ arrow body
- ⚠️ control flow statements
    - ✅ `if` statment
    - ⚠️ loops
        - ✅ `while`
        - ✅ `do-while`
        - ✅ `for`
        - ⚠️ `foreach`
            - ✅ basic `foreach`
            - ❌ `foreach` with deconstruction syntax
    - ❌ `goto` statement
    - ⚠️ `switch` statement
        - ⚠️ case - not all patterns supported
        - ✅ default
- ⚠️ `try-catch`
    - ✅ `try`
    - ✅ `catch`
    - ✅ `finaly`
    - ❌ filter catch
- ✅ return
- ✅ throw
- ✅ break
- ✅ continue
- ✅ local functions
- ❌ yield
- ❌ LINQ
- ❌ using statement
- ❌ checked block
- ❌ unsafe block

### Expressions

- ✅ prefix unary operation expression
- ✅ postfix unary operation expression
    - note: `!` operator is ignored
- ⚠️ binary operation expression
    - ❌ `>>>` operation is not supported
    - note: `??` and `=??` are mapped to ternary expression which do the same thing
- ✅ ternary operation expression
- ✅ assignment expression
- ✅ variable reference expression
- ✅ parenthesized expression
- ⚠️ member access expression - partialy working
- ⚠️ function call - partialy working
    - ⚠️ method call
    - ⚠️ local function call
    - ⚠️ anonymous lambda call
    - ❌ delegate call
- ❌ new expression
    - ❌ new with type name
    - ❌ new without defined type - deduced
    - ❌ object initialization
- ❌ switch expression
- ❌ lambda expression
- ❌ type cast expression
- ❌ member binding expression - `?.` operator
- ✅ `this` expression
- ⚠️ patterns
    - ✅ constant pattern
    - ❌ declaration pattern
    - ❌ relation pattern
    - ❌ and pattern
    - ❌ or pattern
    - ❌ not pattern
    - ❌ recursive pattern
    - ❌ property pattern clause
    - ❌ when clause
    - ❌ is pattern

#### Literals

- ⚠️ integer
    - ❌ usigned
    - ✅ sbyte - mapped to int
    - ✅ short - mapped to int
    - ✅ int
    - ❌ long
- ⚠️ floating point
    - ✅ float
    - ⚠️ double
    - ❌ decimal
- ⚠️ character
    - ✅ 8-bit ASCII char
    - ❌ 16-bit unicode char
- ⚠️ string
    - ✅ basic string
        - ❌ interpolated string
    - ✅ raw string
        - ❌ interpolated raw string
    - ✅ verbatim string
        - ❌ interpolated verbatim string
- ✅ `null` literal

### Other

- ❌ global statements - not really global, C# only calls it as global
    - ❌ global functions
    - ❌ global variables

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

