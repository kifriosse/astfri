# astfri
TODO

# Build
We use submodules for most dependencies. After cloning the repository, they must be initialized and updated:
```sh
git clone --recurse-submodules git@github.com:kifriosse/astfri.git
git submodule update --init --recursive
```

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
If you want to build everything, simply run `./scripts/prepare-build.sh` from the root of the project. It will create `build` and run cmake configuration but won't run build.

`CPP_INPUT` requires LLVM. This is the only dependency that we don't use submodule for because of extensive build time. See [libastfri-cpp/README.md](libastfri-cpp/README.md) for packages that need to be installed.


# For Developers

## File structure
This repository contains multiple libraries. Each library lives in its own directory prefixed with `lib-` (in case there is also a tool with the same name) that is structured as follows:
- `doc/` if present, contains documentation for the library
- `examples/` if present, contains example usage of the library
- `{library}/`
  - has the same name as the library without `lib-` prefix
  - contains include files that should be included by users of the library
  - contains `impl` directory with rest of the source files (`.hpp` and `.cpp`)
  - `impl` directory may be structured further
- `tests/` contains definition of test executable and all supporting files
- `CMakeLists.txt`
  - this one should be clean and easy to read
  - defines the library target and its properties
  - include path of the target should start here (`target_include_directories(astfri-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})`)
    - this keeps the library isolated (setting the include path does not expose anything else)
    - all `#include`s in the library sources should use fully qualified paths (`#include <{library}/impl/header.hpp`) to avoid ambiguities when the library is used in some other project
- `README.md` standard readme file, should contain information on build and dependencies

Use `libastfri` as a reference.

## Notes
- C++ standard is C++20, this is ensured by compiler option inherited from the core library
- Standard complience is required, this is ensured by `-Wpedantic` option that is also inherited
- Class names follow upper [CammelCase](https://en.wikipedia.org/wiki/Camel_case) notation, e.g. `class ForLoop`
- Function names follow [snake_case](https://en.wikipedia.org/wiki/Snake_case) notation, e.g. `create_int`
- Variable names follow lower [cammelCase](https://en.wikipedia.org/wiki/Camel_case) notation, e.g. `variableName`
- Member variables use `m_*` prefix, e.g. `m_statements`
- File names follow upper [CammelCase](https://en.wikipedia.org/wiki/Camel_case) notation, e.g. `ExpressionFactory.hpp`
- Code must compile with at least `-Wall -Wextra -Wpedantic` with no warnings
- Code formatting is ensured by `clang-format` configured in `.clang-format` (for now, it has to be invoked manually -- we will change it to pre-commit hook)
- Declarations go into `*.hpp` file and definition goes into `*.cpp` file with the same stem
- Definitions of templates go into `*.inl` file with the same stem, the `*.inl` file is included at the end of `*.hpp` file, see. e.g. [Expr.hpp](libastfri/astfri/impl/Expr.hpp) and [Expression.inl](libastfri/astfri/impl/Expr.inl)
- Inline definitions also go to the `*.inl` file, do not forget to define them with `inline` if they are not a template
