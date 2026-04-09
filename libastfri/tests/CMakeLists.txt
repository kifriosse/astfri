add_executable(astfri-core-tests)

target_sources(astfri-core-tests PRIVATE
  Main.cpp
  ExprFactoryTest.cpp
  StmtFactoryTest.cpp
  TypeFactoryTest.cpp
  OptPtrTest.cpp
  ReqPtrTest.cpp
)

target_link_libraries(astfri-core-tests PRIVATE astfri-core Catch2)
