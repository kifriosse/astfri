option(CPP_TEST "Build test for CPP lib" ON)

# toto som dal, nech sa ostatným zbytočne nebuilduje môj test
if (CPP_TEST)
    add_subdirectory(main)
endif()

add_subdirectory(astfri-cpp/impl)

target_include_directories(astfri-cpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
