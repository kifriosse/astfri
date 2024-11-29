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
