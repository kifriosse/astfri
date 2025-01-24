# Nastavenie zdrojových súborov
set(SOURCE_FILES_JR
    main.cpp
)

add_executable(test_JR ${SOURCE_FILES_JR})
#add_executable(testASTs_JR created_ASTs.cpp)

target_link_libraries(test_JR astfri-cpp)
#target_link_libraries(testASTs_JR astfri-cpp)
