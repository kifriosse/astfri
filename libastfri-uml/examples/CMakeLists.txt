add_executable(uml-example)
target_sources(uml-example PRIVATE
	example.cpp
)
target_compile_options(uml-example PRIVATE "-Wall" "-Wextra" "-Wpedantic")
target_link_libraries(uml-example PRIVATE astfri-core)

add_executable(uml-class-example)
target_sources(uml-class-example PRIVATE
	example_class.cpp
)
target_compile_options(uml-class-example PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(uml-class-example PRIVATE astfri-core)

add_executable(uml-external-example)
target_sources(uml-external-example PRIVATE
	example_external.cpp
)
target_link_libraries(uml-external-example PRIVATE
	astfri-core
	uml-outputter
	uml-class-visitor
)