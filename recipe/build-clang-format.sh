#!/bin/bash
set -e

# Enforce proper formatting of our source code
conda install -y --quiet clangdev
clang-format -i `git ls-files '*.cc' '*.hpp' '*.ipp' '*.h'`
git diff --exit-code
