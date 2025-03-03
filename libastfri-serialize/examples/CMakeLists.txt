add_executable(testWithTextOutput TestWithTextOutput.cpp)


          
target_compile_options(testWithTextOutput PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(testWithTextOutput PRIVATE astfri-core)
target_link_libraries(testWithTextOutput PRIVATE astfri-text)
target_link_libraries(testWithTextOutput PRIVATE astfri-serialize)



