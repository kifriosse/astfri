add_executable(astfri)

target_sources(astfri PRIVATE
	main.cpp
	args.cpp
	version.cpp)

target_link_libraries(astfri PRIVATE
  astfri-core
	astfri-cpp
	astfri-cs
	astfri-java
	astfri-text
	astfri-uml
	cxxopts)
