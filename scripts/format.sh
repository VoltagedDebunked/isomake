#!/bin/bash

current_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
parent_dir="$(dirname "$current_dir")"
src_dir="$parent_dir/src"

if [ ! -d "$src_dir" ]; then
    echo "Error: src directory not found."
    exit 1
fi

cd "$src_dir" || exit

if [ ! -f "main.c" ]; then
    echo "Error: main.c not found in src directory."
    exit 1
fi

clang-format -i main.c

echo "main.c has been formatted."