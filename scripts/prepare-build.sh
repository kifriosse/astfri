#!/bin/sh

# Choose a compiler
COMPILER=clang++

# Remove old build files
rm -rf build/release
rm -rf build/debug

# Create build directories
mkdir -p build/release
mkdir -p build/debug

# Generate release Makefile
cd build/release
cmake -DCMAKE_CXX_COMPILER=$COMPILER     \
      -DCMAKE_BUILD_TYPE=Release         \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      -DASTFRI_BUILD_CPP_FRONTEND=ON     \
      ../..

# Generate debug Makefile
cd ../debug
cmake -DCMAKE_CXX_COMPILER=$COMPILER     \
      -DCMAKE_BUILD_TYPE=Debug           \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      -DASTFRI_BUILD_CPP_FRONTEND=ON     \
      ../..

# Move compile commnads out of the build directory
mv compile_commands.json ../..