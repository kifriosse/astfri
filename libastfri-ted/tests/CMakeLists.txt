add_executable(astfri-ted-tests)

target_sources(astfri-ted-tests PRIVATE
  Main.cpp
  ZhangShashaTest.cpp
)

target_link_libraries(astfri-ted-tests PRIVATE astfri-ted Catch2)

apply_common_properties(astfri-ted-tests)