add_library(
  astfri-core SHARED
    Expr.cpp
    ExprFactory.cpp
    Stmt.cpp
    StmtFactory.cpp
    Type.cpp
    TypeFactory.cpp
    TypeQueries.cpp
)

target_compile_options(astfri-core PRIVATE -Wall -Wextra -Wpedantic)

target_compile_options(astfri-core PUBLIC -std=c++20)

target_include_directories(astfri-core PUBLIC ${PROJECT_SOURCE_DIR})