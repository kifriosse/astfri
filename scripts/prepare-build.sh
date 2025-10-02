#!/usr/bin/env bash

# Fancy output
C_RED='\033[1;31m'
C_PURPLE='\033[1;35m'
C_GREEN='\033[1;32m'
C_RESET='\033[0m'

# Print error
error() {
  echo -e "${C_RED}Error:${C_RESET} $1"
}

# Print info
info() {
      echo "$1"
}

# Print heading
heading() {
  echo -e "${C_PURPLE}$1${C_RESET}"
}

# Print ok
ok() {
  echo -e "${C_GREEN}Done${C_RESET}"
}

# Die if error
possibly_die() {
  if [ $? = 1 ]; then
    error $1
    exit 1
  fi
}

# Choose a compiler
COMPILER=g++

# Remove old build files
heading "Removing old build files"
rm -rf build/release
rm -rf build/debug
ok
echo ""

# Create build directories
heading "Creating new build directories"
mkdir -p build/release
mkdir -p build/debug
ok
echo ""

# Generate release Makefile
heading "Generating realease config"
cd build/release
cmake -DCMAKE_CXX_COMPILER=$COMPILER     \
      -DCMAKE_BUILD_TYPE=Release         \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      -DASTFRI_BUILD_CPP_INPUT=ON        \
      -DASTFRI_BUILD_JAVA_INPUT=ON       \
      -DASTFRI_BUILD_SERIALIZED_INPUT=ON \
      -DASTFRI_BUILD_TEXT_OUTPUT=ON      \
      -DASTFRI_BUILD_UML_OUTPUT=ON       \
      ../..
possibly_die "CMake for release config failed"
ok
echo ""

# Generate debug Makefile
heading "Generating debug config"
cd ../debug
cmake -DCMAKE_CXX_COMPILER=$COMPILER     \
      -DCMAKE_BUILD_TYPE=Debug           \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      -DASTFRI_BUILD_CPP_INPUT=ON        \
      -DASTFRI_BUILD_JAVA_INPUT=ON       \
      -DASTFRI_BUILD_SERIALIZED_INPUT=ON \
      -DASTFRI_BUILD_TEXT_OUTPUT=ON      \
      -DASTFRI_BUILD_UML_OUTPUT=ON       \
      ../..
possibly_die "CMake for debug config failed"
ok
echo ""

# Move compile commnads out of the build directory
mv compile_commands.json ../..
