#!/bin/bash

# We cannot pipefail here, so if --list-test-names-only fails, we won't ever
# know. The problem here is that Catch decides to abuse the error code and
# returns 34.
set -e

./main --list-test-names-only | parallel --bar --halt-on-error 2 --verbose ./main
