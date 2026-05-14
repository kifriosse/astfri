TODO

## How to load a file and create Translation Unit
```sh
#include <astfri-cpp/AstfriCpp.hpp>
```
```sh
astfri::TranslationUnit tu = astfri::cpp_in::load_file(argv[1], astfri::cpp::Config());
```

## Dependencies
### LLVM and clang
Versions 15 available in Ubuntu 22 and version 19 available in Void Linux both seem to work.

#### Ubuntu
```sh
apt install clang-15 llvm-15-dev libclang-15-dev
```

#### Void Linux
```sh
xbps-install -Su llvm19-devel
```

#### Arch

```sh
sudo pacman -S llvm clang
```
