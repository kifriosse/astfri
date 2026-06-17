# Usage

Your CMake target must link library `astfri-uml`.
- In your header file `#include <astfri-uml/UMLLibWrapper.hpp>`
- Create an instance of `astfri::uml::UMLLibWrapper`.
- Create an instance of:
    1. `astfri::uml::UMLOutputter` (currently only `astfri::uml::PlantUMLOutputter` is implemented)
    2. `astfri::uml::Config`
- Call the init method on your `astfri::uml::UMLLibWrapper` instance.
- Call the run method on your `astfri::uml::UMLLibWrapper` instance.

Note: See [example](examples/ExampleStandalone.cpp) for more details.

# Config
The best way to start is to use the default config created using `Config::create_default`. If you need to adjust something you can modify the config members directly. If you need to persist the changes, you can save the config to a file. Later, it can be loaded using `Config::create_from_json`:
```c++
auto config = astfri::uml::Config::create_default();
config.intTypeName = "integer";
config.write_json("config.json");

auto myConfig = astfri::uml::create_from_json("config.json");
```

Every value of length 1 char in the default config can only support 1 char long strings.
If a longer string is provided, only the first char will be used.

All colors are RGB Hex values (with or without a leading # character).

`typeConvention` currently only accepts values `TypeBeforeIdentifier` (e.g. `int x`) and `TypeAfterIdentifier` (e.g. `x: int`).
