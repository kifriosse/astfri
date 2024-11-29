include(FindPackageHandleStandardArgs)

find_library(
  TREE_SITTER_LIBRARY
    NAMES tree-sitter
    DOC   "tree-sitter library"
)

find_path(
  TREE_SITTER_INCLUDE_DIR
    NAMES api.h
    DOC   "tree-sitter api header"
    PATH_SUFFIXES tree_sitter
)

find_package_handle_standard_args(
  TreeSitter
    REQUIRED_VARS TREE_SITTER_LIBRARY TREE_SITTER_INCLUDE_DIR
)

if (TreeSitter_FOUND)
  add_library(
    tree-sitter UNKNOWN IMPORTED
  )

  set_target_properties(
    tree-sitter PROPERTIES
    IMPORTED_LOCATION ${TREE_SITTER_LIBRARY}
  )
endif()
