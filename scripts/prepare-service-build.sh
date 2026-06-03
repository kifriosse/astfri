#!/usr/bin/env bash

# Include common build utils.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "${SCRIPT_DIR}/prepare-build-common.sh"

choose_compiler
remove_build_dir
create_build_dir

highlight "Service build --> release, static libs, standalone executable."
set -x
cd build
cmake -DCMAKE_C_COMPILER=${C_COMPILER}     \
      -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
      -DCMAKE_BUILD_TYPE=Release           \
      -DBUILD_SHARED_LIBS=OFF              \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON   \
      -DASTFRI_BUILD_CLI=ON                \
      -DASTFRI_BUILD_CPP_INPUT=ON          \
      -DASTFRI_BUILD_JAVA_INPUT=ON         \
      -DASTFRI_BUILD_C_SHARP_INPUT=ON      \
      -DASTFRI_BUILD_SERIALIZE=ON          \
      -DASTFRI_BUILD_TEXT_OUTPUT=ON        \
      -DASTFRI_BUILD_UML_OUTPUT=ON         \
      -DASTFRI_BUILD_TESTS=OFF             \
      -DASTFRI_BUILD_EXAMPLES=OFF          \
      ..
{ set +x; } 2>/dev/null
possibly_die "CMake failed"
ok
echo ""
