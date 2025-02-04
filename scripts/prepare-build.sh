#!/bin/sh

# TODO add proper exit and failure message

# Choose a compiler
COMPILER=g++

# Remove old build files
rm -rf build/release
rm -rf build/debug

# Create build directories
mkdir -p build/release
mkdir -p build/debug

# Generate release Makefile
echo "# Generating realease config"
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
echo ""

# Generate debug Makefile
echo "# Generating debug config"
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

# Move compile commnads out of the build directory
mv compile_commands.json ../..
