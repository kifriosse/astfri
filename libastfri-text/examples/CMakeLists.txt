add_executable(app_mp)

target_sources(app_mp PRIVATE main.cpp)

target_link_libraries(app_mp PRIVATE astfri-text)
