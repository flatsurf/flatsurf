[![Linux](https://img.shields.io/circleci/project/github/polygon-tbd/polygon/master.svg?label=Linux)](https://circleci.com/gh/polygon-tbd/polygon)
[![OSX](https://img.shields.io/travis/polygon-tbd/polygon/master.svg?label=macOS)](https://travis-ci.org/polygon-tbd/polygon)
![Windows disabled](https://img.shields.io/badge/Windows-disabled-lightgrey.svg)
[![Coverage Status](https://coveralls.io/repos/github/polygon-tbd/polygon/badge.svg?branch=_conda_cache_origin_head)](https://coveralls.io/github/polygon-tbd/polygon?branch=_conda_cache_origin_head)
[![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/polygon-tbd/polygon/master?filepath=binder%2FSample.ipynb)
[![asv](http://img.shields.io/badge/benchmarked%20by-asv-blue.svg?style=flat)](https://polygon-tbd.github.io/polygon-asv/)

## Current Release Info

We build and release this package with every push to the master branch. These releases are considered unstable and highly
experimental. There are no stable releases yet.

This repository contains three related projects:

* **libflatsurf** a C++ library
* **pyflatsurf** a Python wrapper for **libflatsurf**
* **flatsurf** a binary that builds upon **libflatsurf**

| Name | Downloads | Version | Platforms |
| --- | --- | --- | --- |
| [![Nightly Build](https://img.shields.io/badge/experimental-libpolygon-green.svg)](https://anaconda.org/saraedum/libpolygon) | [![Conda Downloads](https://img.shields.io/conda/dn/saraedum/libpolygon.svg)](https://anaconda.org/saraedum/libpolygon) | [![Conda Version](https://img.shields.io/conda/vn/saraedum/libpolygon.svg)](https://anaconda.org/saraedum/libpolygon) | [![Conda Platforms](https://img.shields.io/conda/pn/saraedum/libpolygon.svg)](https://anaconda.org/saraedum/libpolygon) |
| [![Nightly Build](https://img.shields.io/badge/experimental-libpypolygon-green.svg)](https://anaconda.org/saraedum/libpypolygon) | [![Conda Downloads](https://img.shields.io/conda/dn/saraedum/libpypolygon.svg)](https://anaconda.org/saraedum/libpypolygon) | [![Conda Version](https://img.shields.io/conda/vn/saraedum/libpypolygon.svg)](https://anaconda.org/saraedum/libpypolygon) | [![Conda Platforms](https://img.shields.io/conda/pn/saraedum/libpypolygon.svg)](https://anaconda.org/saraedum/libpypolygon) |

## Install with Conda

You can install this package with conda. Download and install [Miniconda](https://conda.io/miniconda.html), then run

```
conda config --add channels flatsurf conda-forge
conda install -c saraedum pyflatsurf flatsurf
```

## Run with binder in the Cloud

You can try out the projects in this repository in a very limited environment online by clicking the following links:

* **libflatsurf** [![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/polygon-tbd/polygon/master?filepath=binder%2FSample.libflatsurf.ipynb)
* **pyflatsurf** [![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/polygon-tbd/polygon/master?filepath=binder%2FSample.pyflatsurf.ipynb)
* **flatsurf** [![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/polygon-tbd/polygon/master?filepath=binder%2FSample.flatsurf.ipynb)

## Build from the Source Code Repository

We are following a standard autoconf setup, i.e., you can create the library
`src/libpolygon` and the corresponding Python wrapper `src/libpypolygon` with
the following:

```
git clone https://github.com/polygon-tbd/polygon.git
cd polygon
./bootstrap
./configure
make
```

For best performance run `CXXFLAGS="-O3 -flto" CXX="g++
-fno-semantic-interposition" ./configure` instead of `./configure` as this code
greatly benefits from flto inlining. (Unfortunately, libtool filters out
`-fno-semantic-interposition` as of early 2019 so we can not pass it as part of
`CXXFLAGS`. If you are using clang, `-fno-semantic-interposition` does not seem
to be necessary.) You might want to add `-g3` to the `CXXFLAGS` which does not
hurt performance but gives a better debugging experience. For the best
debugging experience, you might want to replace `-O3` with `-Og` or even `-O0`
but the latter results in very poor performance.

Additionally, you might want to run with configure with ` --disable-static`
which improves the build time.

[perf](https://perf.wiki.kernel.org/index.php/Main_Page) works well to profile
when you make sure that `CXXFLAGS` contains `-fno-omit-frame-pointer`. You can
then for example run our test suite with `perf record --call-graph dwarf make check`
Apart from perf itself there are several ways to analyze the output,
[hotspot](https://github.com/KDAB/hotspot) might be the most convenient one at
the time of this writing.

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
conda create -n polygon-test --use-local libpypolygon
source activate polygon-test
```

## Maintainers

* [@saraedum](https://github.com/saraedum)
