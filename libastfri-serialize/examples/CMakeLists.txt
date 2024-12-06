set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PROJECT_SOURCE_DIR /home/makuch/astfri)

add_executable(example exampleFunction.cpp )



target_include_directories(example PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(example PRIVATE ${PROJECT_SOURCE_DIR}/libastfri-core.so)
