# astfri
TODO

# For Developers

## File structure
This repository contains multiple subprojects. Each subproject lives in its own directory and is structured as follows:
- `doc/` if present, contains documentation for the library/tool
- `examples/` if present, contains example usage of the library/tool
- `impl/` contains source files and internal header files
- `inc/` contains public header files
- `tests/` contains definition of test executable and all supporting files
- `CMakeLists.txt` just includes subdirectories
- `README.md` standard readme file, should contain information on build and dependencies

## Notes
- C++ standard is C++20, this is ensured by `target_compile_features(<target> PUBLIC cxx_std_20)` in cmake  
- Standard complience is required, this is ensured by `set_target_properties(<target> PROPERTIES CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)` in cmake ..
- Class names follow upper [cammel case](https://en.wikipedia.org/wiki/Camel_case) notation, e.g. `class ForLoop`  
- Function names follow [snake case](https://en.wikipedia.org/wiki/Snake_case) notation, e.g. `create_int`  
- Variable names follow lower [cammel case](https://en.wikipedia.org/wiki/Camel_case) notation, e.g. `variableName`  
- Member variables use underscore as suffix, e.g. `statements_`  
- File names follow upper [cammel case](https://en.wikipedia.org/wiki/Camel_case) notation, e.g. `ExpressionFactory.hpp`  
- Code must compile with at least `-Wall -Wextra -Wpedantic` with no warnings  
- Code formatting is ensured by `clang-format` configured in `.clang-format` (for now, it has to be invoked manually -- we will change it to pre-commit hook)
- Declarations go into `*.hpp` file and definition goes into `*.cpp` file with the same stem  
- Definitions of templates go into `*.inl` file with the same stem, the `*.inl` file is included at the end of `*.hpp` file, see. e.g. [Expr.hpp](libastfri/inc/Expr.hpp.hpp) and [Expression.inl](libastfri/impl/Expr.inl)  
- Inline definitions also go to the `*.inl` file, do not forget to define them with `inline` if they are not a template