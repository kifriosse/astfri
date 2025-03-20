add_executable(uml-example)
target_sources(uml-example PRIVATE
	example.cpp
)
target_link_libraries(uml-example PRIVATE
	astfri-uml
    astfri-cpp
)
