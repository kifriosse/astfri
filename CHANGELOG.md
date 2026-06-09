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
