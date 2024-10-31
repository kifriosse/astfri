find_package(LLVM REQUIRED)

set(
  CLANG_LIBS clangTooling clangFrontend clangSerialization clangDriver
    clangParse clangSema clangAnalysis clangEdit clangAST clangLex clangBasic
)

foreach (lib ${CLANG_LIBS})
  find_library(Clang_${lib}_LIBRARY NAMES ${lib} PATHS ${LLVM_LIB_DIR})
  list(APPEND Clang_LIBRARIES ${Clang_${lib}_LIBRARY})
endforeach()

list(APPEND Clang_LIBRARIES ${LLVM_LIBRARIES})
set(Clang_INCLUDE_DIRS ${LLVM_INCLUDE_DIRS})
set(Clang_OPTIONS ${LLVM_OPTIONS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  Clang Clang_LIBRARIES Clang_INCLUDE_DIRS
)
