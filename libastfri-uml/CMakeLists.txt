add_executable(uml-example)
target_sources(uml-example PRIVATE
	main.cpp
)
target_compile_options(uml-example PRIVATE "-Wall" "Wextra" "Wpedantic")
target_link_libraries(uml-example PRIVATE astfri-core)