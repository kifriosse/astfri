#!/usr/bin/env bash

# Include io utils.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "${SCRIPT_DIR}/fancy-io.sh"

help_and_die() {
  echo "Provide zero arguments and gcc and g++ will be used."
  echo "Provide two arguments and they will be used as C and C++ compilers."
  echo "Anything else is an error."
  exit 1
}

choose_compiler() {
  heading "Choosing compilers"
  if [ "$#" -eq 0 ]; then
    C_COMPILER=gcc
    CXX_COMPILER=g++
    echo "Using defaults -- gcc and g++."
    { set +x; } 2>/dev/null
    ok
    echo ""
  elif [ "$#" -eq 2 ]; then
    C_COMPILER=$1
    CXX_COMPILER=$2
    echo "Using ${C_COMPILER} as C compiler and ${CXX_COMPILER} as C++ compiler."
    { set +x; } 2>/dev/null
    ok
    echo ""
  else
    error "Invalid arguments."
    help_and_die
  fi
}

remove_build_dir() {
  heading "Removing old build files"
  set -x
  rm -rf build
  { set +x; } 2>/dev/null
  ok
  echo ""
}

create_build_dir() {
  heading "Creating new build directories"
  set -x
  mkdir -p build
  { set +x; } 2>/dev/null
  ok
  echo ""
}
