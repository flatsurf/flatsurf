#!/bin/bash
set -exo pipefail

unset ASV_SECRET_KEY

make check

# Report coverage data to coveralls
conda install -y --quiet pip git gcc_linux-64
# Allow installation of cpp-coveralls with pip (otherwise disabled during conda builds)
unset PIP_NO_INDEX
unset PIP_NO_DEPENDENCIES
pip install cpp-coveralls

cd $SRC_DIR/src/libflatsurf/

git remote remove origin || true
git remote add origin https://github.com/flatsurf/flatsurf.git
git fetch origin
git checkout -b master
git branch -u origin/master
export CI="true"
export CI_NAME="conda-build"
export CI_BUILD_NUMBER="$GIT_DESCRIBE_NUMBER"
export CI_BUILD_URL="https://dev.azure.com/flatsurf/conda/_build?definitionId=3&_a=summary"
export CI_BRANCH="master"

set +x
if [ ${#COVERALLS_REPO_TOKEN} = 33 ];then coveralls --gcov `which x86_64-conda_cos6-linux-gnu-gcov` --exclude flatsurf/external --gcov-options '\-lrp' -b .; fi
