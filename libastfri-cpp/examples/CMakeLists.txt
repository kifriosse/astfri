# Nastavenie zdrojových súborov
set(SOURCE_FILES_JR
    main.cpp
)

add_executable(test_JR ${SOURCE_FILES_JR})

# Linkovanie s astfri-cpp
target_link_libraries(test_JR astfri-cpp)

# MM: linkovanim astfri-cpp sa tranzitivne linkuje aj astfri-core a clang
