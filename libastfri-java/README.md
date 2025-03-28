TODO


## Dependencies
### Tree-sitter
Install tree-sitter from source:
```sh
git clone https://github.com/tree-sitter/tree-sitter.git
cd tree-sitter
git checkout e445532a1fea3b1dda93cee61c534f5b9acc9c16
cd lib
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
sudo make install
```

Install java grammar for tree sitter:
```sh
git clone https://github.com/tree-sitter/tree-sitter-java.git
cd tree-sitter-java
git checkout a1bbe92a6370bb4c15386735fbda12f2b812a923
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
sudo make install
```

# Usage
Link library astfri-java in CMakeLists.txt

1. Include >> #include <libastfri-java/inc/ASTBuilder.hpp>
2. Make a instance of ASTBuilder that has parameterless constructor.
3. Then call one of functions:
    3.1 Function load_file that returns string and takes string parameter for path to file (if you want to load one specific file).
    3.2 Function load_directory that returns string and takes string parameter for path to project directory (if you want to load all .java files from project).
4. Then call function make_syntax_tree that returns syntax tree (type TSTree* from tree-sitter library) and takes string parameter that is source code from one of functions from step 3.

Look into libastfri-java/examples/example_text_output.cpp for example code for text output.
Look into libastfri-java/examples/example_uml_output.cpp for example code for uml output.