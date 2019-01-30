#!/bin/bash
set -eo pipefail

# Check that the tests actually pass before we attempt to profile them
./test/test.sh dev

# We collect test results in an Airspeed Velocity database if we have the necessary credentials
if [ -n "$ASV_SECRET_KEY" ];then
  conda install --quiet -y git
  git config --global user.name 'CircleCI Benchmark'
  git config --global user.email 'benchmark@circleci.invalid'
  git config --global push.default nothing
  mkdir -p ~/.ssh
  sudo yum install -y openssh-clients
  ssh-keyscan -H github.com >> ~/.ssh/known_hosts

  set +x
  echo $ASV_SECRET_KEY | base64 -d > ~/.ssh/id_rsa
  chmod 400 ~/.ssh/id_rsa
  set -x

  cd /home/conda/feedstock_root

  # Clone performance data of previous runs
  rm -rf .asv/results
  git clone git@github.com:polygon-tbd/polygon-asv.git .asv/results

  ./test/test.sh run
  
  # We publish test results as an Airspeed Velocity database
  pushd .asv/results
  git add .
  git commit -m "Added benchmark run"
  unset SSH_AUTH_SOCK
  git push origin master:master
  popd

  asv gh-pages --no-push
  git push git@github.com:polygon-tbd/polygon-asv.git gh-pages:gh-pages -f
fi
