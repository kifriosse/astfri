add_executable(testWithTextOutput)

target_sources(testWithTextOutput PRIVATE TestWithTextOutput.cpp)

target_link_libraries(testWithTextOutput PRIVATE astfri-serialize astfri-text)


add_executable(testWithUmlOutput)

target_sources(testWithUmlOutput PRIVATE TestWithUmlOutput.cpp)

target_link_libraries(testWithUmlOutput PRIVATE astfri-serialize astfri-uml)

