#!/usr/bin/env bash

# Print error
error() {
  echo -e "${C_RED}Error:${C_RESET} $1"
}

files=$(find libastfri* -type f -name "*.cpp" -or -name "*.hpp" -or -name "*.inl")

for file in ${files}; do
    clang-format -style=file --dry-run ${file} > /dev/null 2>&1
    if [ "$?" -eq "0" ]; then
        echo "Formatting ${file}"
        clang-format -i -style=file ${file}
    else
        error "Failed to run format on ${file}"
        echo "Try running: "
        echo "  clang-format -i -style=file ${file}"
        echo "and check the output."
        exit 1
    fi
done
