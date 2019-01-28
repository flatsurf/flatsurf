#!/bin/bash
set -e

# Go to the test directory, so we don't have to worry about path names
cd -P -- "$(dirname -- "$0")"

conda install --quiet -y valgrind

# Run a simple case with valgrind to detect memory leaks (eventually, we should
# run all tests but with a timeout.)
valgrind --leak-check=full polygon 1 1 2 2 --depth=10 --follow_depth=50 --show_cyls --show_moduli --cyls_only --quiet

./test-build.sh
