# astfri
TODO





# Build


## Dependencies
We use submodules for most dependencies. After cloning the repository, they must
be initialized and updated:
```sh
git clone --recurse-submodules git@github.com:kifriosse/astfri.git
git submodule update --init --recursive
```
The `libastfri-cpp` target requires LLVM. This is the only dependecny not
installed from a submodule (because of long build times). It must be installed
in the system:

**Ubuntu 22**
```sh
apt install clang-15 llvm-15-dev libclang-15-dev
```

**Void Linux**
```sh
xbps-install -Su llvm19-devel
```

**Arch**
```sh
sudo pacman -S llvm clang
```

**MSYS2/UCRT64**
```sh
pacman -S \
  mingw-w64-clang-x86_64-llvm-libs \
  mingw-w64-clang-x86_64-llvm-tools \
  mingw-w64-ucrt-x86_64-llvm \
  mingw-w64-ucrt-x86_64-llvm-libs \
  mingw-w64-ucrt-x86_64-llvm-tools \
  mingw-w64-ucrt-x86_64-clang \
  mingw-w64-ucrt-x86_64-clang-libs \
  mingw-w64-ucrt-x86_64-clang-tools-extra \
  mingw-w64-ucrt-x86_64-tree-sitter
  # Installing tree-sitter should not be necessary, but it is for now...
```


## Compiler
The project requires gcc and g++ in version 12 or higher. On in some systems
(Ubuntu 22), they need to be explicitly installed using `apt install gcc-12
g++-12`.

Clang might also work, but is not tested yet.


## Platforms
Project builds on typical Linux distributions and on Windows with
[MSYS2(UCRT64)](https://www.msys2.org/) (other MSYS2 environments should also
work, but are not tested yet). MacOS should also work but is not tested yet.


## CMake
The project follows standard cmake build procedure. By default, only the
`libastfri-core` target is enabled. Other targets can be enabled using the
following cmake options:
- `ASTFRI_BUILD_CLI`
- `ASTFRI_BUILD_SERVICE`
- `ASTFRI_BUILD_IN_CPP`
- `ASTFRI_BUILD_IN_JAVA`
- `ASTFRI_BUILD_IN_CSHARP`
- `ASTFRI_BUILD_OUT_TEXT`
- `ASTFRI_BUILD_OUT_UML`
- `ASTFRI_BUILD_SERIALIZED`

by setting their valuet to `ON`. Tests and examples are disabled by default.
They can be enabled using the following options:
- `ASTFRI_BUILD_TESTS`
- `ASTFRI_BUILD_EXAMPLES`

by setting them to the value `ON`. For example, to build the project with java
frontend library on Ubuntu 22:
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

Convenient scripts for building different project configurations are available
in `scripts/`. For example, to build everything for development purposes run:
```
./scripts/prepare-devel-build.sh
```
This configuration is the most stable and tested one.


## Binary Artefacts
Runnable binaries and libraries are all built into `build/out/bin`. To run
an example do:
```sh
cd build/out/bin
./astfri-uml-example-cs ../../../resources/samples/csharp/Matrix.cs
```





# For Developers


## File structure
This repository contains multiple libraries. Each library lives in its own
directory prefixed with `lib-` (in case there is also a tool with the same name)
that is structured as follows:
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
  - include path of the target should start here
  (`target_include_directories(astfri-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})`)
    - this keeps the library isolated (setting the include path does not expose
    anything else)
    - all `#include`s in the library sources should use fully qualified paths
    (`#include <{library}/impl/header.hpp`) to avoid ambiguities when the
    library is used in some other project
- `README.md` standard readme file, should contain library usage information

Use `libastfri-core` as a reference.


## Notes
- C++ standard is C++20, this is ensured by compiler option set for each target
- Standard complience is required, this is ensured by `-Wpedantic` option that
is set for each target
- Class names follow upper
[CammelCase](https://en.wikipedia.org/wiki/Camel_case) notation, e.g. `class
ForLoop`
- Function names follow [snake_case](https://en.wikipedia.org/wiki/Snake_case)
notation, e.g. `create_int`
- Variable names follow lower
[cammelCase](https://en.wikipedia.org/wiki/Camel_case) notation, e.g.
`variableName`
- Member variables use `m_*` prefix, e.g. `m_statements`
- File names follow upper [CammelCase](https://en.wikipedia.org/wiki/Camel_case)
notation, e.g. `ExpressionFactory.hpp`
- Code must compile with at least `-Wall -Wextra -Wpedantic` with no warnings
- Code formatting is ensured by `clang-format` configured in `.clang-format`
(for now, it has to be invoked manually -- we will change it to pre-commit hook)
- Declarations go into `*.hpp` file and definition goes into `*.cpp` file with
the same stem
- Definitions of templates go into `*.inl` file with the same stem, the `*.inl`
file is included at the end of `*.hpp` file, see. e.g.
[Expr.hpp](libastfri/astfri/impl/Expr.hpp) and
[Expression.inl](libastfri/astfri/impl/Expr.inl)
- Inline definitions also go to the `*.inl` file, do not forget to define them
with `inline` if they are not a template
