#add_subdirectory(examples)
add_subdirectory(impl)
add_subdirectory(examples)

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    add_subdirectory(tests)
endif ()

