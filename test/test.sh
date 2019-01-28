#!/bin/bash
set -exo pipefail

asv machine --machine circleci --yes

# Unfortunately, asv does not set its exit code when one of the tests
# failed, so we to parse its output
asv "$@" --machine circleci | tee /tmp/asv.out
! grep failed /tmp/asv.out > /dev/null
