#!/bin/bash
set -eo pipefail
./main --list-test-names-only | parallel --bar --halt now,fail=1 ./main
