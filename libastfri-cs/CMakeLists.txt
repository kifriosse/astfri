add_library(astfri-cs SHARED)

add_subdirectory(impl)
add_subdirectory(examples)

add_custom_command(
    TARGET astfri-cs POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/libastfri-cs/resources/"
    "${CMAKE_BINARY_DIR}/libastfri-cs/resources"
    COMMENT "Coping astfri-cs resources"
)

target_compile_definitions(astfri-cs PRIVATE
    ASTFRI_CS_RESOURCES="${CMAKE_BINARY_DIR}/libastfri-cs/resources/external-types/"
)