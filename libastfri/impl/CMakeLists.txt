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

target_compile_features(astfri-core PUBLIC cxx_std_20)

target_compile_options(astfri-core PRIVATE -Wall -Wextra -Wpedantic)

target_include_directories(astfri-core PUBLIC ${PROJECT_SOURCE_DIR})