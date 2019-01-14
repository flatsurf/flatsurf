#!/bin/bash
# We should use proper unit tests that test bits of the library instead.
# However, until jrueth understands what the individual parts do, we decided to
# just run some cases that are fast to see that we did not break anything.
set -exo pipefail

# Go to the test directory, so the path names are correct
cd -P -- "$(dirname -- "$0")"

for output in *.sample.out; do
    diff <(polygon `basename "$output" .sample.out`) "$output";
done
