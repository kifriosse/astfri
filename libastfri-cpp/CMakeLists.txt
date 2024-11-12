find_package(Clang REQUIRED)

add_library(
  astfri-cpp SHARED
    ClangManagement.cpp
    ClangVisitor.cpp
)

target_compile_options(
  astfri-cpp PRIVATE ${Clang_OPTIONS} -Wall -Wextra -Wpedantic
)

target_compile_options(
  astfri-cpp PUBLIC -std=c++20
)

target_include_directories(
  astfri-cpp SYSTEM PUBLIC ${Clang_INCLUDE_DIRS}
)

target_include_directories(
  astfri-cpp PUBLIC ${PROJECT_SOURCE_DIR}
)

target_link_libraries(
  astfri-cpp PUBLIC ${Clang_LIBRARIES} astfri-core
)
