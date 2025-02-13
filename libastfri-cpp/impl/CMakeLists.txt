# Find LLVM
find_package(LLVM REQUIRED)

# Find Clang
find_package(Clang REQUIRED)

# Obtain LLVM options from their config tool
execute_process(
  COMMAND llvm-config --cxxflags
  OUTPUT_VARIABLE COMPILE_OPTIONS_PUBLIC
)

# Remove trailing and leading spaces, most importantly, newlines!
string(STRIP "${COMPILE_OPTIONS_PUBLIC}" COMPILE_OPTIONS_PUBLIC)

# Replace spaces with ; to turn it into list
# target_* functions work better with lists---
# avoiding quotes and ugly spaces
string(REGEX REPLACE " +" ";" COMPILE_OPTIONS_PUBLIC "${COMPILE_OPTIONS_PUBLIC}")

# If the first option is include path, remove it
# It is set later from LLVM_INCLUDE_DIRS as system include to surpress warnings
list(GET COMPILE_OPTIONS_PUBLIC 0 FIRST_OPTION)
if (FIRST_OPTION MATCHES "-I.*")
  list(REMOVE_AT COMPILE_OPTIONS_PUBLIC 0)
endif()
unset(FIRST_OPTION)

# Append no rtti option
list(APPEND COMPILE_OPTIONS_PUBLIC -fno-rtti)

# Append our options
list(APPEND COMPILE_OPTIONS_PUBLIC -std=c++20)

# Define and append our private options
list(APPEND COMPILE_OPTIONS_PRIVATE -Wall -Wextra -Wpedantic)

# Finally, add our library and set its properties
add_library(
  astfri-cpp SHARED
    ClangManagement.cpp
    ClangVisitor.cpp
)

# Public compile options
target_compile_options(
  astfri-cpp PUBLIC ${COMPILE_OPTIONS_PUBLIC}
)

# Private compile options
target_compile_options(
  astfri-cpp PRIVATE ${COMPILE_OPTIONS_PRIVATE}
)

# Set LLVM include as system to avoid warnings from their code
target_include_directories(
  astfri-cpp SYSTEM PUBLIC ${LLVM_INCLUDE_DIRS}
)

# Link the core module and clang libraries
target_link_libraries(
  astfri-cpp PUBLIC
    astfri-core
    clang
    clangAST
    clangASTMatchers
    clangBasic
    clangFrontend
    clangTooling
)
