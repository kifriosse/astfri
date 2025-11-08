#add_subdirectory(examples)
add_subdirectory(impl)
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    add_subdirectory(tests)
endif()

