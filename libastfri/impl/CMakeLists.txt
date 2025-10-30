add_library(astfri-core SHARED)

target_sources(astfri-core PRIVATE
    Expr.cpp
    ExprFactory.cpp
    Scope.cpp
    Stmt.cpp
    StmtFactory.cpp
    Type.cpp
    TypeFactory.cpp
)

target_compile_options(astfri-core PUBLIC
    -Wall
    -Wextra
    -Wpedantic
    -std=c++20
)

target_include_directories(astfri-core PUBLIC ${PROJECT_SOURCE_DIR})
