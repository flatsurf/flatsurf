[![Linux](https://dev.azure.com/flatsurf/conda/_apis/build/status/flatsurf.flatsurf?branchName=master&jobName=linux&configuration=linux%20build_linux_)](https://dev.azure.com/flatsurf/conda/_build/latest?definitionId=&branchName=master)
![OSX disabled](https://img.shields.io/badge/OSX-disabled-lightgrey.svg)
![Windows disabled](https://img.shields.io/badge/Windows-disabled-lightgrey.svg)
![ppc64le disabled](https://img.shields.io/badge/ppc64le-disabled-lightgrey.svg)
[![codecov](https://codecov.io/gh/flatsurf/flatsurf/branch/master/graph/badge.svg)](https://codecov.io/gh/flatsurf/flatsurf)
[![asv](http://img.shields.io/badge/benchmarked%20by-asv-blue.svg?style=flat)](https://flatsurf.github.io/flatsurf-asv/)

## Current Release Info

We build and release this package with every push to the master branch. These releases are considered unstable and highly
experimental. There are no stable releases yet.

This repository contains three related projects:

* **libflatsurf** a C++ library
* **pyflatsurf** a Python wrapper for **libflatsurf**
* **flatsurf-polygonn** a legacy binary that builds upon **libflatsurf**

| Name | Downloads | Version | Platforms |
| --- | --- | --- | --- |
| [![Nightly Build](https://img.shields.io/badge/recipe-libflatsurf-green.svg)](https://anaconda.org/flatsurf/libflatsurf) | [![Conda Downloads](https://img.shields.io/conda/dn/flatsurf/libflatsurf.svg)](https://anaconda.org/flatsurf/libflatsurf) | [![Conda Version](https://img.shields.io/conda/vn/flatsurf/libflatsurf.svg)](https://anaconda.org/flatsurf/libflatsurf) | [![Conda Platforms](https://img.shields.io/conda/pn/flatsurf/libflatsurf.svg)](https://anaconda.org/flatsurf/libflatsurf) |
| [![Nightly Build](https://img.shields.io/badge/recipe-pyflatsurf-green.svg)](https://anaconda.org/flatsurf/pyflatsurf) | [![Conda Downloads](https://img.shields.io/conda/dn/flatsurf/pyflatsurf.svg)](https://anaconda.org/flatsurf/pyflatsurf) | [![Conda Version](https://img.shields.io/conda/vn/flatsurf/pyflatsurf.svg)](https://anaconda.org/flatsurf/pyflatsurf) | [![Conda Platforms](https://img.shields.io/conda/pn/flatsurf/pyflatsurf.svg)](https://anaconda.org/flatsurf/pyflatsurf) |
| [![Nightly Build](https://img.shields.io/badge/recipe-flatsurf-green.svg)](https://anaconda.org/flatsurf/flatsurf) | [![Conda Downloads](https://img.shields.io/conda/dn/flatsurf/flatsurf.svg)](https://anaconda.org/flatsurf/flatsurf) | [![Conda Version](https://img.shields.io/conda/vn/flatsurf/flatsurf.svg)](https://anaconda.org/flatsurf/flatsurf) | [![Conda Platforms](https://img.shields.io/conda/pn/flatsurf/flatsurf.svg)](https://anaconda.org/flatsurf/flatsurf) |

## Install with Conda

You can install these packages with conda. Download and install [Miniconda](https://conda.io/miniconda.html), then run

```
conda config --add channels conda-forge
conda create -n flatsurf -c flatsurf flatsurf-polygon pyflatsurf
conda activate flatsurf
```

The Python wrapper `pyflatsurf` is based on [cppyy](https://cppyy.readthedocs.io/) which is [not available on conda-forge yet](https://bitbucket.org/wlav/cppyy/issues/55/package-for-conda-forge). Therefore, to use the Python wrapper, you need to install cppyy from PyPI:

```
conda install pip
pip install cppyy
```

## Run with binder in the Cloud

You can try out the projects in this repository in a very limited environment online by clicking the following links:

* **libflatsurf** [![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/flatsurf/flatsurf/master?filepath=binder%2FSample.libflatsurf.ipynb)
* **pyflatsurf** [![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/flatsurf/flatsurf/master?filepath=binder%2FSample.pyflatsurf.ipynb)
* **flatsurf-polygon** [![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/flatsurf/flatsurf/master?filepath=binder%2FSample.flatsurf-polygon.ipynb)

## Build from the Source Code Repository

We are following a standard autoconf setup, i.e., you can create the library
`src/libflatsurf`, the Python wrapper `src/pyflatsurf`, and the binary
`src/flatsurf-polygon/flatsurf-polygon` with the following:

```
git clone --recurse-submodules https://github.com/flatsurf/flatsurf.git
cd flatsurf
./bootstrap
./configure
make
make check # to run our test suite
make install # to install into /usr/local
```

For best performance run `CXXFLAGS="-O3 -flto -march=native -mtune=native"
CXX="g++ -fno-semantic-interposition" ./configure` instead of `./configure` as
this code greatly benefits from flto inlining. (Unfortunately, libtool filters
out `-fno-semantic-interposition` as of early 2019 so we can not pass it as
part of `CXXFLAGS`. If you are using clang, `-fno-semantic-interposition` does
not seem to be necessary.) Do not use `-Ofast` or `-ffast-math` as parts of our
code rely on IEEE compliance. You might want to add `-g3` to the `CXXFLAGS`
which does not hurt performance but gives a better debugging experience. For
the best debugging experience, you might want to replace `-O3` with `-Og` or
even `-O0` but the latter results in very poor performance.

Additionally, you might want to run with configure with ` --disable-static`
which improves the build time.

[perf](https://perf.wiki.kernel.org/index.php/Main_Page) works well to profile
when you make sure that `CXXFLAGS` contains `-fno-omit-framepointer`. You can
then for example run our test suite with `perf record --call-graph dwarf make
check` Apart from perf itself there are several ways to analyze the output,
[hotspot](https://github.com/KDAB/hotspot) might be the most convenient one at
the time of this writing.

## Build from the Source Code Repository with Conda Dependencies

To build these packages, you need a fairly recent C++ compiler and probably
some packages that might not be readily available on your system. If you don't
want to use your distribution's packages, you can provide these dependencies
with conda. Download and install [Miniconda](https://conda.io/miniconda.html),
then run

```
conda config --add channels conda-forge
conda config --add channels flatsurf # if you want to pull in the latest version of dependencies
conda create -n flatsurf-build cxx-compiler libtool automake boost-cpp libexactreal gtest benchmark ccache
conda activate flatsurf-build
export CPPFLAGS="-isystem $CONDA_PREFIX/include"
export CFLAGS="$CPPFLAGS"
export LDFLAGS="-L$CONDA_PREFIX/lib -Wl,-rpath-link=$CONDA_PREFIX/lib"
export CC="ccache cc"
export CXX="ccache c++"
git clone --recurse-submodules https://github.com/flatsurf/flatsurf.git
cd flatsurf
./bootstrap
./configure --prefix="$CONDA_PREFIX"
make
```

## Build from the Source Code Repository with Conda

The conda recipe in `recipe/` is built automatically as part of our Continuous
Integration. If you want to build the recipe manually, something like the
following should work:

```
git clone --recurse-submodules https://github.com/flatsurf/flatsurf.git
cd flatsurf
conda activate root
conda config --add channels conda-forge
conda config --add channels flatsurf # if you want to pull in the latest version of dependencies
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
conda create -n flatsurf-test --use-local libflatsurf pyflatsurf flatsurf-polygon
conda activate flatsurf-test
```

## Run Tests and Benchmark

`make check` runs all tests and benchmarks. During development `make check TESTS=module`
only runs the tests for `module`. For randomized tests, you might want to add
`GTEST_REPEAT=1024` to run such tests repeatedly. Note that the environment
variable `EXACTREAL_CHECK` is passed on to the tests and benchmarks, i.e., you
could add `EXACTREAL_CHECK="--benchmark_min_time=.02"` to not let the
benchmarks run as long as they would usually.

## Maintainers

* [@saraedum](https://github.com/saraedum)
* [@videlec](https://github.com/videlec)
