#!/usr/bin/env bash

# Fancy output
C_RED='\033[1;31m'
C_PURPLE='\033[1;35m'
C_GREEN='\033[1;32m'
C_YELLOW='\033[1;33m'
C_RESET='\033[0m'

# Print error
error() {
  echo -e "${C_RED}Error:${C_RESET} $1"
}

# Print info
info() {
  echo "$1"
}

# Print heading
heading() {
  echo -e "# ${C_PURPLE}$1${C_RESET}"
}

# Print highligthed text.
highlight() {
  echo -e "${C_YELLOW}$1${C_RESET}"
}

# Print ok
ok() {
  echo -e "${C_GREEN}Done${C_RESET}"
}

# Die if error
possibly_die() {
  if [ $? = 1 ]; then
    error $1
    exit 1
  fi
}
