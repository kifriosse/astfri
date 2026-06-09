add_executable(astfri-service)

target_sources(astfri-service PRIVATE main.cpp)

target_link_libraries(astfri-service PRIVATE
  astfri-core
	astfri-cpp
	astfri-cs
	astfri-java
	astfri-text
	astfri-uml
	cxxopts
)
