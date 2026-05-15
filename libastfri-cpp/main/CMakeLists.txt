add_executable(main_CPP)

target_sources(main_CPP PRIVATE main.cpp)

target_link_libraries(main_CPP PRIVATE astfri-cpp astfri-text astfri-uml)
