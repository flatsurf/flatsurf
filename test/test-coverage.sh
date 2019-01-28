#!/bin/bash
set -e

# Go to the test directory, so we don't have to worry about path names
cd -P -- "$(dirname -- "$0")"

# Restore metadata that gcov needs to make sense of the coverage results
tar xvvf /tmp/artifacts.tar

./test-build.sh

# Report coverage data to coveralls
conda install -y --quiet pip git gcc_linux-64
pip install pyyaml cpp-coveralls
cd $CONDA_BLD_PATH/polygon*/work
set +x
if [ -n "$COVERALLS_REPO_TOKEN" ];then coveralls --gcov `which x86_64-conda_cos6-linux-gnu-gcov` --gcov-options '\-lrp' -b src; fi
