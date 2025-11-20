Library AST FRI for C\#
===

- Supported C# versions by Tree-Sitter C# grammar: C# 1 - C# 13.0
- TODO subproject description

Build
---

- TODO

Dependencies
---

- Tree-Sitter library (commit `e445532a1fea3b1dda93cee61c534f5b9acc9c16`)
- Tree-Sitter C# Grammar (commit `6563c3af3f03ec948d08f9325e4bbd072e4c6b99`)

### Dependencies for building Tree-Sitter and Tree-Sitter C# Grammar

- GCC / G++ (C/C++ compiler)
- Make (build tool)
- CMake ≥ 3.13 (minimal version for Tree-Sitter and Tree-Sitter C# Grammar)
    - to verify if you already have these tools on your system
    ```sh
    gcc --version      # Check GCC / G++
    make --version     # Check Make
    cmake --version    # Check CMake, also to verify if you have minimal version installed
    ```

Arch
---

### Install GCC + Make + CMake

```sh
sudo pacman -Syu    # Update system
sudo pacman -S base-devel cmake
# or
sudo pacman -S --needed base-devel cmake # will only install packages that are not installed and will try to update already installed packages
```

Ubuntu/Debian
---

### Install GCC + Make + CMake

```sh
sudo apt update
sudo apt upgrade
sudo apt install build-essential cmake
```

Linux distribution independent
---

### Installing Tree Sitter Library

```sh
cd ~
git clone https://github.com/tree-sitter/tree-sitter.git
cd tree-sitter
git checkout e445532a1fea3b1dda93cee61c534f5b9acc9c16
cd lib
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
sudo make install
```

### Installing Tree Sitter C\# Grammar

```sh
cd ~
git clone https://github.com/tree-sitter/tree-sitter-c-sharp.git
cd tree-sitter
git checkout 6563c3af3f03ec948d08f9325e4bbd072e4c6b99
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
sudo make install
```