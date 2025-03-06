## How to use 
1.include header for AstFriSerialiesr: 
```cpp
#include <libastfri-serialize/inc/AstFriSerializer.hpp>
```
2.AstFriSerialiser is singleton - to create object do this:
```cpp
AstFriSerializer& serializer = AstFriSerializer::get_instance();
```
3.To serialize objects from JSON to ASTFRI use method serialize,which takes one argument of type std::string-path to JSON file,method returns astfri::IVisitable* type,which is root of AST from JSON,
in following example argument passed to serialise method is second argument of program
```cpp
astfri::IVisitable* node = serializer.serialize(argv[1]);
```
Look at whole example [here](https://github.com/kifriosse/astfri/blob/dev-jm/libastfri-serialize/examples/TestWithTextOutput.cpp)

4.This library submodule target is called astfri-serialize 

 To link it with your target use 
 ```text
target_link_libraries(YourTarget PRIVATE astfri-serialize)
 ```

### JSON representation of ASTFRI nodes
[see](https://github.com/kifriosse/astfri/blob/dev-jm/libastfri-serialize/docs/Template-JSON-representation-of-ASTFRI-nodes.txt)


## Build
 
```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DASTFRI_BUILD_TEXT_OUTPUTT=ON ..
cmake -DCMAKE_BUILD_TYPE=Debug -DASTFRI_BUILD_UML_OUTPUT=ON ..
cmake -DCMAKE_BUILD_TYPE=Debug -DASTFRI_BUILD_SERIALIZED_INPUT=ON ..
make
```
Note:Text and UML output submodules are built because of targets defined in examples subfolder(used for testing) 


## Dependencies
This library submodoule for serialisation json format into ASTFRI is dependent on:

[astfri-core](https://github.com/kifriosse/astfri/tree/main/libastfri) - core of the project containing functionality to work with ASTFRI 

[rapidjson](https://github.com/Tencent/rapidjson) - used for parsing json file

```cmake
target_link_libraries(astfri-serialize PUBLIC astfri-core)
target_include_directories(astfri-serialize PUBLIC ${PROJECT_SOURCE_DIR}/lib)
```



