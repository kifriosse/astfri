add_library(
    astfri-core DYNAMIC
        impl/Declaration.cpp
        impl/DeclarationFactory.cpp
        impl/Expression.cpp
        impl/ExpressionFactory.cpp
        impl/Statement.cpp
        impl/StatementFactory.cpp
        impl/Type.cpp
        impl/TypeFactory.cpp
)

target_compile_options(astfri-core PRIVATE -Wall -Wextra -Wpedantic)

target_include_directories(astfri-core PUBLIC ${PROJECT_SOURCE_DIR})