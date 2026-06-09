add_executable(astfri-cli)

target_sources(astfri-cli PRIVATE
	main.cpp
	args.cpp
	version.cpp)

target_link_libraries(astfri-cli PRIVATE
  astfri-core
	astfri-cpp
	astfri-cs
	astfri-java
	astfri-serialize
	astfri-text
	astfri-uml
	cxxopts)
