add_executable(app_mp main.cpp)

target_compile_options(app_mp PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(app_mp PRIVATE astfri-core)