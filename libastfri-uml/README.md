# Usage

Your CMake target must link library "astfri-uml".

- In your header file `#include <libastfri-uml/inc/UMLLibWrapper.hpp>`
- Create an instance of uml::UMLLibWrapper
- Create an instance of:
    1. uml::UMLOutputter (currently only PlantUMLOutputter is implemented)
    2. uml::Config (json loading currently not implemented, only default config)
    3. uml::TypeConvention (TypeBeforeConvention/TypeAfterConvention)
- Call the init method on your uml::UMLLibWrapper instance
- Call the run method on your uml::UMLLibWrapper instance

Note: see [this example (line 120)](examples/example_external.cpp)