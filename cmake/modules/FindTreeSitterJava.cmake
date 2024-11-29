include(FindPackageHandleStandardArgs)

find_library(
  TREE_SITTER_JAVA_LIBRARY
    NAMES tree-sitter-java
    DOC   "tree-sitter-java library"
)

find_path(
  TREE_SITTER_JAVA_INCLUDE_DIR
    NAMES tree-sitter-java.h
    DOC   "tree-sitter-java header"
    PATH_SUFFIXES tree_sitter
)

find_package_handle_standard_args(
  TreeSitterJava
    REQUIRED_VARS TREE_SITTER_JAVA_LIBRARY TREE_SITTER_JAVA_INCLUDE_DIR
)

if (TreeSitterJava_FOUND)
  add_library(
    tree-sitter-java UNKNOWN IMPORTED
  )

  set_target_properties(
    tree-sitter-java PROPERTIES
    IMPORTED_LOCATION ${TREE_SITTER_JAVA_LIBRARY}
  )
endif()
