# Nastavenie zdrojových súborov
set(SOURCE_FILES_JR
    main.cpp
)

add_executable(test_JR ${SOURCE_FILES_JR})
add_executable(testASTs_JR created_ASTs.cpp)


# Find LLVM and Clang
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

# Linkovanie s LLVM a Clang knižnicami a s astfri-cpp
target_link_libraries(test_JR 
    astfri-cpp
    clangAST
    clangBasic
    clangFrontend
    clangTooling
    ${LLVM_LIBRARIES}
)
target_link_libraries(testASTs_JR astfri-cpp)

# MM: linkovanim astfri-cpp sa tranzitivne linkuje aj astfri-core a clang
