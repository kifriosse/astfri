#!/usr/bin/env bash

# Print help and die
help_and_die() {
  echo "Provide zero arguments and gcc and g++ will be used."
  echo "Provide two arguments and they will be used as C and C++ compilers."
  echo "Anything else is an error."
  exit 1
}

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

# Choose compiler
heading "Choosing compilers"
if [ "$#" -eq 0 ]; then
  C_COMPILER=gcc
  CXX_COMPILER=g++
  echo "Using defaults -- gcc and g++."
  echo ""
elif [ "$#" -eq 2 ]; then
  C_COMPILER=$1
  CXX_COMPILER=$2
  echo "Using ${C_COMPILER} as C compiler and ${CXX_COMPILER} as C++ compiler."
  echo ""
else
  error "Invalid arguments."
  help_and_die
fi

# Remove old build files
heading "Removing old build files"
set -x
rm -rf build
{ set +x; } 2>/dev/null
ok
echo ""

# Initialize and update submodules
heading "Initializing and updating submodules"
set -x
git submodule update --init --recursive
{ set +x; } 2>/dev/null
possibly_die "Git submodule update failed"
ok
echo ""

# Remove external bin files
heading "Removing external bin files"
set -x
rm -rf external/bin
{ set +x; } 2>/dev/null
ok
echo ""

# Create build directories
heading "Creating new build directories"
set -x
mkdir -p build
{ set +x; } 2>/dev/null
ok
echo ""

# Generate debug Makefile
heading "Generating debug config"
set -x
cd build
cmake -DCMAKE_C_COMPILER=${C_COMPILER}     \
      -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
      -DCMAKE_BUILD_TYPE=Debug             \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON   \
      -DASTFRI_BUILD_CPP_INPUT=ON          \
      -DASTFRI_BUILD_JAVA_INPUT=ON         \
      -DASTFRI_BUILD_C_SHARP_INPUT=ON      \
      -DASTFRI_BUILD_SERIALIZED_INPUT=ON   \
      -DASTFRI_BUILD_TEXT_OUTPUT=ON        \
      -DASTFRI_BUILD_UML_OUTPUT=ON         \
      ..
{ set +x; } 2>/dev/null
possibly_die "CMake for debug config failed"
ok
echo ""
