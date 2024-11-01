# Nastavenie zdrojových súborov
set(SOURCE_FILES_JR
    main.cpp
)

add_executable(test ${SOURCE_FILES_JR})

## Nastavenie cesty k LLVM/Clang
set(CLANG_LIBRARY_DIR "/usr/lib/llvm-18/lib/")
set(CLANG_INCLUDE_DIR "/usr/lib/llvm-18/include/")

# Pridanie cesty k libclang
link_directories(${CLANG_LIBRARY_DIR})
include_directories(${CLANG_INCLUDE_DIR})


# Vyhľadanie libclang pomocou find_library1
find_library(CLANG_LIB clang HINTS ${CLANG_LIBRARY_DIR})

if(NOT CLANG_LIB)
    message(FATAL_ERROR "libclang not found")
endif()

# Linkovanie s libclang
target_link_libraries(test ${CLANG_LIB} astfri-core)