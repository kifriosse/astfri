execute_process(
  COMMAND llvm-config --cxxflags
  OUTPUT_VARIABLE LLVM_OPTIONS
)

execute_process(
  COMMAND llvm-config --includedir
  OUTPUT_VARIABLE LLVM_INCLUDE_DIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND llvm-config --libdir
  OUTPUT_VARIABLE LLVM_LIB_DIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND llvm-config --version
  OUTPUT_VARIABLE LLVM_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND llvm-config --system-libs
  OUTPUT_VARIABLE LLVM_SYSTEM_LIBS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND llvm-config --libs
  OUTPUT_VARIABLE LLVM_LIBS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

if (NOT "${LLVM_LIBS}" STREQUAL "" AND NOT "${LLVM_SYSTEM_LIBS}" STREQUAL "")
  set(LLVM_LIBRARIES ${LLVM_LIBS} ${LLVM_SYSTEM_LIBS})
endif()

if (NOT "${LLVM_INCLUDE_DIR}" STREQUAL "")
  set(LLVM_INCLUDE_DIRS ${LLVM_INCLUDE_DIR})
endif()

set(LLVM_OPTIONS ${LLVM_OPTIONS} "-fno-rtti")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  LLVM DEFAULT_MSG LLVM_LIBRARIES LLVM_INCLUDE_DIRS
)