find_package(LLVM REQUIRED)

# We certainly do not need all of these
# Someone should identify which ones we need
set(
  CLANG_LIBS
    clang
    clangAPINotes
    clangARCMigrate
    clangAST
    clangASTMatchers
    clangAnalysis
    clangAnalysisFlowSensitive
    clangApplyReplacements
    clangBasic
    clangChangeNamespace
    clangCodeGen
    clangCrossTU
    clangDaemon
    clangDaemonTweaks
    clangDependencyScanning
    clangDirectoryWatcher
    clangDoc
    clangDriver
    clangDynamicASTMatchers
    clangEdit
    clangFormat
    clangFrontend
    clangFrontendTool
    clangHandleCXX
    clangHandleLLVM
    clangIncludeFixer
    clangIncludeFixerPlugin
    clangIndex
    clangIndexSerialization
    clangInterpreter
    clangLex
    clangMove
    clangParse
    clangQuery
    clangReorderFields
    clangRewrite
    clangRewriteFrontend
    clangSema
    clangSerialization
    clangStaticAnalyzerCheckers
    clangStaticAnalyzerCore
    clangStaticAnalyzerFrontend
    clangTooling
    clangToolingASTDiff
    clangToolingCore
    clangToolingInclusions
    clangToolingRefactoring
    clangToolingSyntax
    clangTransformer
    clangdRemoteIndex
    clangdSupport
)

foreach (lib ${CLANG_LIBS})
  find_library(Clang_${lib}_LIBRARY NAMES ${lib} PATHS ${LLVM_LIB_DIR})
  list(APPEND Clang_LIBRARIES ${Clang_${lib}_LIBRARY})
endforeach()

list(APPEND Clang_LIBRARIES ${LLVM_LIBRARIES})
list(APPEND Clang_INCLUDE_DIRS ${LLVM_INCLUDE_DIRS})
list(APPEND Clang_OPTIONS ${LLVM_OPTIONS})

# If the first option is include path, remove it
# It is set later from LLVM_INCLUDE_DIRS as system include to surpress warnings
list(GET Clang_OPTIONS 0 Clang_FIRST_OPTION)
if (Clang_FIRST_OPTION MATCHES "-I.*")
  list(REMOVE_AT Clang_OPTIONS 0)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  Clang Clang_LIBRARIES Clang_INCLUDE_DIRS
)
