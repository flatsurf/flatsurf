[![Linux](https://img.shields.io/circleci/project/github/polygon-tbd/polygon/master.svg?label=Linux)](https://circleci.com/gh/polygon-tbd/polygon)
[![OSX](https://img.shields.io/travis/polygon-tbd/polygon/master.svg?label=macOS)](https://travis-ci.org/polygon-tbd/polygon)
![Windows disabled](https://img.shields.io/badge/Windows-disabled-lightgrey.svg)
[![Coverage Status](https://coveralls.io/repos/github/polygon-tbd/polygon/badge.svg?branch=_conda_cache_origin_head)](https://coveralls.io/github/polygon-tbd/polygon?branch=_conda_cache_origin_head)
[![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/polygon-tbd/polygon/master?filepath=binder%2FSample.ipynb)
[![asv](http://img.shields.io/badge/benchmarked%20by-asv-blue.svg?style=flat)](https://polygon-tbd.github.io/polygon-asv/)

## Current Release Info

We build and release this package with every push to the master branch. These releases are considered unstable and highly
experimental. There are no stable releases yet.

| Name | Downloads | Version | Platforms |
| --- | --- | --- | --- |
| [![Nightly Build](https://img.shields.io/badge/experimental-polygon-green.svg)](https://anaconda.org/saraedum/polygon) | [![Conda Downloads](https://img.shields.io/conda/dn/saraedum/polygon.svg)](https://anaconda.org/saraedum/polygon) | [![Conda Version](https://img.shields.io/conda/vn/saraedum/polygon.svg)](https://anaconda.org/saraedum/polygon) | [![Conda Platforms](https://img.shields.io/conda/pn/saraedum/polygon.svg)](https://anaconda.org/saraedum/polygon) |
| [![Nightly Build](https://img.shields.io/badge/experimental-libpolygon-green.svg)](https://anaconda.org/saraedum/libpolygon) | [![Conda Downloads](https://img.shields.io/conda/dn/saraedum/libpolygon.svg)](https://anaconda.org/saraedum/libpolygon) | [![Conda Version](https://img.shields.io/conda/vn/saraedum/libpolygon.svg)](https://anaconda.org/saraedum/libpolygon) | [![Conda Platforms](https://img.shields.io/conda/pn/saraedum/libpolygon.svg)](https://anaconda.org/saraedum/libpolygon) |

## Install with Conda

You can install this package with conda. Download and install [Miniconda](https://conda.io/miniconda.html), then run

```
conda config --add channels conda-forge
conda install -c saraedum polygon
```

## Run with binder in the Cloud

You can try out this library in a very limited environment online by clicking
[![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/polygon-tbd/polygon/master?filepath=binder%2FSample.ipynb)

## Build from the Source Code Repository

We are following a standard autoconf setup, i.e., you can create the executable `src/polygon` with the following:

```
git clone https://github.com/polygon-tbd/polygon.git
cd polygon
./bootstrap
./configure
make
```

## Build from the Source Code Repository with Conda

The conda recipe in `recipe/` is built automatically as part of our Continuous
Integration. If you want to build the recipe manually, something like the
following should work:

```
git clone https://github.com/polygon-tbd/polygon.git
cd polygon
source activate root
conda config --add channels conda-forge
conda install conda-build conda-forge-ci-setup=2
export FEEDSTOCK_ROOT=`pwd`
export RECIPE_ROOT=${FEEDSTOCK_ROOT}/recipe
export CI_SUPPORT=${FEEDSTOCK_ROOT}/.ci_support
export CONFIG=linux_
make_build_number "${FEEDSTOCK_ROOT}" "${RECIPE_ROOT}" "${CI_SUPPORT}/${CONFIG}.yaml"
conda build "${RECIPE_ROOT}" -m "${CI_SUPPORT}/${CONFIG}.yaml" --clobber-file "${CI_SUPPORT}/clobber_${CONFIG}.yaml"
```

You can then try out the package that you just built with:
```
conda create -n polygon-test --use-local polygon
source activate polygon-test
```

## Maintainers

* [@saraedum](https://github.com/saraedum)
