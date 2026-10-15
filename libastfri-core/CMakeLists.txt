add_library(astfri-core)

target_sources(astfri-core PRIVATE
  astfri/impl/ExprFactory.cpp
  astfri/impl/Scope.cpp
  astfri/impl/StmtFactory.cpp
  astfri/impl/TypeFactory.cpp
  astfri/impl/Visitor.cpp
)

target_sources(astfri-core
  PUBLIC FILE_SET headers
    TYPE HEADERS
    FILES
      astfri/Astfri.hpp
      astfri/impl/ASTNode.hpp
      astfri/impl/ASTNode.inl
      astfri/impl/Concepts.hpp
      astfri/impl/ExprDef.hpp
      astfri/impl/ExprFactory.hpp
      astfri/impl/ExprFwd.hpp
      astfri/impl/ExprKind.hpp
      astfri/impl/Kind.hpp
      astfri/impl/Scope.hpp
      astfri/impl/Scope.inl
      astfri/impl/StmtDef.hpp
      astfri/impl/StmtFactory.hpp
      astfri/impl/StmtFwd.hpp
      astfri/impl/StmtKind.hpp
      astfri/impl/TypeDef.hpp
      astfri/impl/TypeFactory.hpp
      astfri/impl/TypeFwd.hpp
      astfri/impl/TypeInfo.hpp
      astfri/impl/TypeInfo.inl
      astfri/impl/TypeInfoFwd.hpp
      astfri/impl/TypeKind.hpp
      astfri/impl/Utils.hpp
      astfri/impl/Visitor.hpp
      astfri/impl/tools/Either.hpp
      astfri/impl/tools/Exceptions.hpp
      astfri/impl/tools/OptPtr.hpp
      astfri/impl/tools/OptPtr.inl
      astfri/impl/tools/ReqPtr.hpp
      astfri/impl/tools/ReqPtr.inl
)

target_compile_definitions(astfri-core PUBLIC ASTFRI_VERSION="${ASTFRI_VERSION}")

target_include_directories(astfri-core PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

apply_common_properties(astfri-core)

if (ASTFRI_BUILD_TESTS)
  add_subdirectory(tests)
endif()
