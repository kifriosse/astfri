# Obtain LLVM cmake dir from their config tool.
execute_process(
    COMMAND llvm-config --cmakedir
    OUTPUT_VARIABLE ASTFRI_CPP_LLVM_CMAKEDIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Derive Clang cmake dir from the LLVM one.
get_filename_component(
    ASTFRI_CPP_LLMV_BASEDIR "${ASTFRI_CPP_LLVM_CMAKEDIR}" DIRECTORY
)
set(ASTFRI_CPP_CLANG_CMAKEDIR "${ASTFRI_CPP_LLMV_BASEDIR}/clang")

# Extend cmake search path with LLVM dir and Clang dir with their *.cmake files.
list(APPEND CMAKE_PREFIX_PATH "${ASTFRI_CPP_LLVM_CMAKEDIR}")
list(APPEND CMAKE_PREFIX_PATH "${ASTFRI_CPP_CLANG_CMAKEDIR}")

# Find LLVM.
find_package(LLVM REQUIRED CONFIG)

# Find Clang.
find_package(Clang REQUIRED CONFIG)

# Obtain LLVM options from their config tool.
execute_process(
    COMMAND llvm-config --cxxflags
    OUTPUT_VARIABLE ASTFRI_CPP_COMPILE_OPTIONS_PRIVATE
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Replace spaces with ; to turn it into list
# target_* functions work better with lists---
# avoiding quotes and ugly spaces.
string(
    REGEX REPLACE " +" ";"
        ASTFRI_CPP_COMPILE_OPTIONS_PRIVATE
        "${ASTFRI_CPP_COMPILE_OPTIONS_PRIVATE}"
)

# Remove include path, it is set later as system include.
list(FILTER ASTFRI_CPP_COMPILE_OPTIONS_PRIVATE EXCLUDE REGEX "-I.*")

# Remove standard, it is inherited from astfri-core.
list(FILTER ASTFRI_CPP_COMPILE_OPTIONS_PRIVATE EXCLUDE REGEX "-std=.*")

# Append no rtti option.
list(APPEND ASTFRI_CPP_COMPILE_OPTIONS_PRIVATE -fno-rtti)

# Set the version of our library.
set(ASTFRI_CPP_VERSION "0.0.2")

# Finally, add our library.
add_library(astfri-cpp)

# Add sources.
target_sources(astfri-cpp PRIVATE
    astfri-cpp/impl/ClangManagement.cpp
    astfri-cpp/impl/visitor-methods/ClangVisitor.cpp
    astfri-cpp/impl/visitor-methods/Types.cpp
    astfri-cpp/impl/visitor-methods/Declarations.cpp
    astfri-cpp/impl/visitor-methods/Statements.cpp
    astfri-cpp/impl/visitor-methods/Expressions.cpp
    astfri-cpp/impl/visitor-methods/Helpers.cpp
    astfri-cpp/impl/visitor-methods/Literals.cpp
    astfri-cpp/impl/visitor-methods/Operators.cpp
)

# Add headers.
target_sources(astfri-cpp PUBLIC FILE_SET headers TYPE HEADERS FILES
    astfri-cpp/impl/ClangManagement.hpp
    astfri-cpp/impl/visitor-methods/ClangVisitor.hpp
    astfri-cpp/AstfriCpp.hpp
)

# Make the version macro available in the code.
target_compile_definitions(astfri-cpp PUBLIC ASTFRI_CPP_VERSION="${ASTFRI_CPP_VERSION}")

# Private compile options.
target_compile_options(astfri-cpp PRIVATE ${ASTFRI_CPP_COMPILE_OPTIONS_PRIVATE})

# Set LLVM include as system to avoid warnings from their code.
target_include_directories(astfri-cpp SYSTEM PUBLIC ${LLVM_INCLUDE_DIRS})

# Set current source dir as include path for our lib.
target_include_directories(astfri-cpp PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Link the core module and clang libraries.
target_link_libraries(astfri-cpp PUBLIC
    astfri-core
    clang-cpp
    LLVM
)

if (ASTFRI_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
