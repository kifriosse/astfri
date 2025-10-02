add_executable(test_JR)

target_sources(test_JR PRIVATE main.cpp)

target_link_libraries(test_JR PRIVATE astfri-cpp astfri-text astfri-uml)
