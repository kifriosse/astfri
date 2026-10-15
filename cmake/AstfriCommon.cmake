set(ASTFRI_LIBPATH "${PROJECT_BINARY_DIR}/out/${CMAKE_INSTALL_LIBDIR}")
set(ASTFRI_BINPATH "${PROJECT_BINARY_DIR}/out/${CMAKE_INSTALL_BINDIR}")
set(ASTFRI_INCPATH "${PROJECT_BINARY_DIR}/out/${CMAKE_INSTALL_INCLUDEDIR}")

function(apply_common_properties target)
  set_target_properties(${target} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${ASTFRI_BINPATH}" # .exe, .cll
    LIBRARY_OUTPUT_DIRECTORY "${ASTFRI_LIBPATH}" # .so / .dylib
    ARCHIVE_OUTPUT_DIRECTORY "${ASTFRI_LIBPATH}" # .a / .lib
    CXX_STANDARD             20
    CXX_STANDARD_REQUIRED    ON
    CXX_EXTENSIONS           OFF
  )

  target_compile_options(${target} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
  )
endfunction()
