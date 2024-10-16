if (ASTFRI_BUILD_CPP_FRONTEND)
    add_executable(clangtest clang-test.cpp)
    target_link_libraries(clangtest PRIVATE astfri-cpp)
endif()


# if (ASTFRI_BUILD_CPP_FRONTEND)
#     add_executable(
#         astfri-example-cpp cpp_frontend.cpp
#     )

#     target_compile_options(
#         astfri-example-cpp PRIVATE
#     )

#     target_link_libraries(
#         astfri-example-cpp PRIVATE astfri-cpp astfri-output
#     )



# endif()

# add_executable(
#     astfri-example-manual manual_structure.cpp
# )

# target_compile_options(
#     astfri-example-manual PRIVATE
# )

# target_link_libraries(
#     astfri-example-manual PRIVATE astfri-output
# )

