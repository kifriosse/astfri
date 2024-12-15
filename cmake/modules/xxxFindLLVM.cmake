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

# Remove trailing and leading spaces, most importantly, newlines!
string(STRIP "${LLVM_OPTIONS}" LLVM_OPTIONS)
string(STRIP "${LLVM_INCLUDE_DIR}" LLVM_INCLUDE_DIR)
string(STRIP "${LLVM_LIB_DIR}" LLVM_LIB_DIR)
string(STRIP "${LLVM_VERSION}" LLVM_VERSION)
string(STRIP "${LLVM_SYSTEM_LIBS}" LLVM_SYSTEM_LIBS)
string(STRIP "${LLVM_LIBS}" LLVM_LIBS)

# Replace spaces with ; to turn it into list
# target_* functions work better with lists---
# avoiding quotes and ugly spaces
string(REGEX REPLACE " +" ";" LLVM_OPTIONS "${LLVM_OPTIONS}")

list(APPEND LLVM_INCLUDE_DIRS ${LLVM_INCLUDE_DIR})
list(APPEND LLVM_LIBRARIES ${LLVM_SYSTEM_LIBS})
list(APPEND LLVM_LIBRARIES ${LLVM_LIBS})
list(APPEND LLVM_OPTIONS "-fno-rtti")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  LLVM DEFAULT_MSG LLVM_LIBRARIES LLVM_INCLUDE_DIRS
)