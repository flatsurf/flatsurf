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

set +x
if [ ${#COVERALLS_REPO_TOKEN} = 36 ];then
  bash <(curl -s https://codecov.io/bash) -t $COVERALLS_REPO_TOKEN -x `which x86_64-conda_cos6-linux-gnu-gcov` -a '\-lrp' -p .
fi
