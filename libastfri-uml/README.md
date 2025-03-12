# Usage

Your CMake target must link library "astfri-uml".

- In your header file `#include <libastfri-uml/inc/UMLLibWrapper.hpp>`
- Create an instance of astfri::uml::UMLLibWrapper
- Create an instance of:
    1. astfri::uml::UMLOutputter (currently only PlantUMLOutputter is implemented)
    2. astfri::uml::Config (json loading currently not implemented, only default config)
    3. astfri::uml::TypeConvention (TypeBeforeConvention/TypeAfterConvention)
- Call the init method on your astfri::uml::UMLLibWrapper instance
- Call the run method on your astfri::uml::UMLLibWrapper instance

Note: see [this example (line 130)](examples/example.cpp)