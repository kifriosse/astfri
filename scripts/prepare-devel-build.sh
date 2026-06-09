#!/usr/bin/env bash

# Include common build utils.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "${SCRIPT_DIR}/prepare-build-common.sh"

choose_compiler
remove_build_dir
create_build_dir

highlight "Devel build --> debug, shared libs, examples, tests."
set -x
cd build
cmake -DCMAKE_C_COMPILER=${C_COMPILER}     \
      -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
      -DCMAKE_BUILD_TYPE=Debug             \
      -DBUILD_SHARED_LIBS=ON               \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON   \
      -DASTFRI_BUILD_CLI=ON                \
      -DASTFRI_BUILD_SERVICE=ON            \
      -DASTFRI_BUILD_IN_CPP=ON             \
      -DASTFRI_BUILD_IN_JAVA=ON            \
      -DASTFRI_BUILD_IN_CSHARP=ON          \
      -DASTFRI_BUILD_OUT_TEXT=ON           \
      -DASTFRI_BUILD_OUT_UML=ON            \
      -DASTFRI_BUILD_SERIALIZE=ON          \
      -DASTFRI_BUILD_TESTS=ON              \
      -DASTFRI_BUILD_EXAMPLES=ON           \
      ..
{ set +x; } 2>/dev/null
possibly_die "CMake failed"
ok
echo ""
