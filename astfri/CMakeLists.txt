add_executable(astfri)

target_sources(astfri PRIVATE main.cpp)

target_link_libraries(astfri PRIVATE
  astfri-core
	astfri-cpp
	astfri-cs
	astfri-java
	astfri-text
	astfri-uml
)

target_include_directories(astfri PRIVATE ${CXXOPTS_INCLUDE_DIRS})
