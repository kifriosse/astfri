TODO

## How to load a file and create Translation Unit
```sh
#include <astfri-cpp/AstfriCpp.hpp>
```
```sh
astfri::TranslationUnit tu = astfri::cpp_in::load_file(argv[1], astfri::cpp::Config());
```