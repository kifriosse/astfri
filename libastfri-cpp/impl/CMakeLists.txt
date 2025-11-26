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

# Finally, add our library and set its properties.
add_library(astfri-cpp SHARED
    ClangManagement.cpp
    ClangVisitor.cpp
    ClangVisitor_Types.cpp
    ClangVisitor_Declarations.cpp
    ClangVisitor_Statements.cpp
    ClangVisitor_Expressions.cpp
    ClangVisitor_Helpers.cpp
    ClangVisitor_Literals.cpp
    ClangVisitor_Operators.cpp
)

# Privae compile options.
target_compile_options(astfri-cpp PRIVATE ${ASTFRI_CPP_COMPILE_OPTIONS_PRIVATE})

# Set LLVM include as system to avoid warnings from their code.
target_include_directories(astfri-cpp SYSTEM PUBLIC ${LLVM_INCLUDE_DIRS})

# Link the core module and clang libraries.
target_link_libraries(astfri-cpp PUBLIC
    astfri-core
    clang
    clangAST
    clangASTMatchers
    clangBasic
    clangFrontend
    clangTooling
)
