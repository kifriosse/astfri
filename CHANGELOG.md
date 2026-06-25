**2026-06-24** `0.1.1` Michal Mrena `<michal.mrena@fri.uniza.sk>`
- `astfri-core` extended forward declarations.
  - Moved version to `0.1.1`.
  - `StmtFwd.hpp` now forward-declares access modifier enum and `StmtFactory`.
  - `ExprFwd.hpp` now forward-declares binary and unary operator types `ExprFactory`.
  - `TypeFwd.hpp` now forward-declares `TypeFactory`.
- `astfri-java` refactored library structures.
  - Moved version to `0.1.1`.
  - Added UML diagram into `doc/`.
  - TODO


**2026-06-17** `0.1.0` Michal Mrena `<michal.mrena@fri.uniza.sk>`
- Unified directory structure of libraries.
  - Each one has single (except `libastfri-cs`) "flattened" `CMakeLists.txt`, which defines the target and its sources (including headers (for installation)).
  - Include path for each library starts in its root directory i.e., include paths look like `<astfri-cpp/AstfriCpp.hpp>`.
- `astfri-uml` refactored config.
  - Removed default config json file and moved it to the code.
  - Unified key and values names in json and code.
  - Refactored examples accordingly.
- `astfri-common` added new library.
  - Contains code shared by multiple libraries.
  - Currently, contains rapidjson utilities.
- `astfri-text` refactored config.
  - Removed default config json file and moved it to the code.
  - Unified key and values names in json and code.
  - Refactored examples accordingly.
  - Added example with bubble sort.
- Refactored examples in each library to use the same conventions.


**2026-06-09** Michal Mrena `<michal.mrena@fri.uniza.sk>`
- Added this `CHANGELOG.md` to track notable changes.
- Added version variables to root `CMakeLists.txt` and root `CMakeLists.txt` of each library.
- Set each version to `0.0.2` as a starting point.
  - Versions follow [semantic versioning](https://semver.org/).
  - MAJOR version `0` means that ***anything MAY change at any time. The public API SHOULD NOT be considered stable***.
  - Individual libraries and executables increase their version independently.
  - Each such increase increases corresponding global version.
  - Version summary can be printed by runing `./astfri-cli/astfri-cli --version` from `build`.
- Extended library concept in [Concepts.hpp](libastfri-core/astfri/impl/Concepts.hpp) by adding requirement on `version` function returning version of the library as string.
