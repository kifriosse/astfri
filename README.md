# astfri
TODO

# TODO
submodule commands doc

# Modules and build
The project requires gcc,g++ in version 12 or higher. On Ubuntu 22, they need to be explicitly installed using `apt install gcc-12 g++-12`.

The project follows standard cmake build procedure. By default, only the `libastfri-core` library is enabled. Other libraries can be enabled using the following cmake options:
- `ASTFRI_BUILD_CPP_INPUT`
- `ASTFRI_BUILD_JAVA_INPUT`
- `ASTFRI_BUILD_SERIALIZED_INPUT`
- `ASTFRI_BUILD_TEXT_OUTPUT`
- `ASTFRI_BUILD_UML_OUTPUT`  

by setting them to the value `ON`. For example, to build the project with java frontend library on Ubuntu 22:
```sh
mkdir build
cd build
cmake                          \
  -DCMAKE_C_COMPILER=gcc-12    \
  -DCMAKE_CXX_COMPILER=g++-12  \
  -DCMAKE_BUILD_TYPE=Debug     \
  -DASTFRI_BUILD_JAVA_INPUT=ON \
   ..
make
```


# Dependencies
Each subproject contains section on dependencies in its README.


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
- C++ standard is C++20, this is ensured by compiler option inherited from the core library
- Standard complience is required, this is ensured by `-Wpedantic` option that is also inherited
- Class names follow upper [cammel case](https://en.wikipedia.org/wiki/Camel_case) notation, e.g. `class ForLoop`
- Function names follow [snake case](https://en.wikipedia.org/wiki/Snake_case) notation, e.g. `create_int`
- Variable names follow lower [cammel case](https://en.wikipedia.org/wiki/Camel_case) notation, e.g. `variableName`
- Member variables use underscore as suffix, e.g. `statements_`
- File names follow upper [cammel case](https://en.wikipedia.org/wiki/Camel_case) notation, e.g. `ExpressionFactory.hpp`
- Code must compile with at least `-Wall -Wextra -Wpedantic` with no warnings
- Code formatting is ensured by `clang-format` configured in `.clang-format` (for now, it has to be invoked manually -- we will change it to pre-commit hook)
- Declarations go into `*.hpp` file and definition goes into `*.cpp` file with the same stem
- Definitions of templates go into `*.inl` file with the same stem, the `*.inl` file is included at the end of `*.hpp` file, see. e.g. [Expr.hpp](libastfri/inc/Expr.hpp) and [Expression.inl](libastfri/impl/Expr.inl)  
- Inline definitions also go to the `*.inl` file, do not forget to define them with `inline` if they are not a template
