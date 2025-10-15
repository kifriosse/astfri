option(CPP_TEST "Build test for CPP lib" OFF)

# toto som dal, nech sa ostatným zbytočne nebuilduje môj test
if (CPP_TEST)
    add_subdirectory(examples)
endif()

add_subdirectory(impl)
