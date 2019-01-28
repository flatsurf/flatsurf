#!/bin/bash
set -e

# Enforce proper formatting of our source code
conda install -y --quiet clangdev
clang-format -i --style=Google src/*.cc src/libpolygon/*.h
git diff --exit-code
