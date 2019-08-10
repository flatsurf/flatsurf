#!/bin/bash
set -exo pipefail

# Create coverage statistics
make CXXFLAGS="$CXXFLAGS $EXTRA_CXXFLAGS"

# Install gcov
conda install -y --quiet gcc_linux-64

cd $SRC_DIR

git remote remove origin || true
git remote add origin https://github.com/flatsurf/flatsurf.git
git fetch origin
git checkout -b master
git branch -u origin/master

set +x
# Do not upload; print what would have been uploaded instead…
COVERALLS_FLAGS="-d"

# The token is only available to builds on the main repository. We could
# potentially change this so it is available to builds by trusted users. Setting
#   VCS_PULL_REQUEST: $(System.PullRequest.PullRequestId)
#   VCS_BRANCH_NAME: $(System.PullRequest.TargetBranch)
# in azure-pipelines-linux.yml, we should get coverage reports on PRs. However,
# then the BINSTAR token must not be used to upload to the default tag on
# anaconda but to some specialized PR tag.

if [ ${#COVERALLS_REPO_TOKEN} = 36 ];then
  # … unless we have the token, then actually upload.
  COVERALLS_FLAGS="-t $COVERALLS_REPO_TOKEN"
fi

bash <(curl -s https://codecov.io/bash) $COVERALLS_FLAGS -R `pwd` -x `which x86_64-conda_cos6-linux-gnu-gcov` -a '\-lrp' -p .