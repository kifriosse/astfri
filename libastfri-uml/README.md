# Usage

Your CMake target must link library "astfri-uml".

- In your header file `#include <libastfri-uml/inc/UMLLibWrapper.hpp>`
- Create an instance of astfri::uml::UMLLibWrapper
- Create an instance of:
    1. astfri::uml::UMLOutputter (currently only astfri::uml::PlantUMLOutputter is implemented)
    2. astfri::uml::Config
- Call the init method on your astfri::uml::UMLLibWrapper instance
- Call the run method on your astfri::uml::UMLLibWrapper instance

Note: see [this example (line 113)](examples/umlexample.cpp)

# Config

See [the default config](examples/default_config.json)

Every value of length 1 char in the default config can only support 1 char long strings.
If a longer string is provided, only the first char will be used.

All colors are RGB Hex values (with or without a leading # character).

`type_convention` currently only accepts values `TypeBeforeConvention` and `TypeAfterConvention`. 
