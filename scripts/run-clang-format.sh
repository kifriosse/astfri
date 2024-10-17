#!/bin/bash

files=$(find libastfri -type f -name "*.cpp" -or -name "*.hpp" -or -name "*.inl")

for file in ${files}; do
    clang-format -style=file --dry-run ${file} > /dev/null 2>&1
    if [[ $? -eq 0 ]]; then
        echo "Formatting ${file}"
        clang-format -i -style=file ${file}
    fi
done