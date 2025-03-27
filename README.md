![Test](https://github.com/flatsurf/flatsurf/workflows/Test/badge.svg)
[![codecov](https://codecov.io/gh/flatsurf/flatsurf/branch/master/graph/badge.svg)](https://codecov.io/gh/flatsurf/flatsurf)
[![asv](http://img.shields.io/badge/benchmarked%20by-asv-blue.svg?style=flat)](https://flatsurf.github.io/flatsurf/asv/)
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.4006152.svg)](https://doi.org/10.5281/zenodo.4006152)

<p align="center">
    <img alt="logo" src="https://avatars.githubusercontent.com/u/46691578?s=300&u=c65d11085307f34e1998cc996e6ee2d16b4d458a&v=4">
</p>

<h1><p align="center">libflatsurf & pyflatsurf</p></h1>

libflatsurf is C++ library to work with [translation surfaces](https://en.wikipedia.org/wiki/Translation_surface) and pyflatsurf is a low-level wrapper to use libflatsurf from Python.

If you came here looking for the [**flatsurf project**](https://flatsurf.github.io), you probably want to start with [sage-flatsurf](https://github.com/flatsurf/sage-flatsurf) which provides a unified interface to these and many other libraries in the [SageMath](https://sagemath.org) computer algebra system.

## Current Release Info

We release this package regularly with [rever](https://regro.github.io/rever-docs/index.html); typically with every push to the master branch.

This repository contains two related projects:

* **libflatsurf** a C++ library
* **pyflatsurf** a Python wrapper for **libflatsurf**

| Name | Downloads | Version | Platforms |
| --- | --- | --- | --- |
| [![Build](https://img.shields.io/badge/recipe-libflatsurf-green.svg)](https://anaconda.org/flatsurf/libflatsurf) | [![Conda Downloads](https://img.shields.io/conda/dn/flatsurf/libflatsurf.svg)](https://anaconda.org/flatsurf/libflatsurf) | [![Conda Version](https://img.shields.io/conda/vn/flatsurf/libflatsurf.svg)](https://anaconda.org/flatsurf/libflatsurf) | [![Conda Platforms](https://img.shields.io/conda/pn/flatsurf/libflatsurf.svg)](https://anaconda.org/flatsurf/libflatsurf) |
| [![Build](https://img.shields.io/badge/recipe-pyflatsurf-green.svg)](https://anaconda.org/flatsurf/pyflatsurf) | [![Conda Downloads](https://img.shields.io/conda/dn/flatsurf/pyflatsurf.svg)](https://anaconda.org/flatsurf/pyflatsurf) | [![Conda Version](https://img.shields.io/conda/vn/flatsurf/pyflatsurf.svg)](https://anaconda.org/flatsurf/pyflatsurf) | [![Conda Platforms](https://img.shields.io/conda/pn/flatsurf/pyflatsurf.svg)](https://anaconda.org/flatsurf/pyflatsurf) |

## Install with Conda

You can install this package with conda. Download and install [Mambaforge](https://github.com/conda-forge/miniforge#mambaforge), then run

```
mamba create -n flatsurf -c flatsurf libflatsurf pyflatsurf
conda activate flatsurf
```

## Build from the Source Code Repository

We are following a standard autoconf setup, i.e., you can create the library
`libflatsurf/src`, and the Python wrapper `pyflatsurf/src` with the following:

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

Additionally, you might want to run configure with ` --disable-static`
which improves the build time.

[perf](https://perf.wiki.kernel.org/index.php/Main_Page) works well to profile
when you make sure that `CXXFLAGS` contains `-fno-omit-frame-pointer`. You can
then for example run our test suite with `perf record --call-graph dwarf make
check` Apart from perf itself there are several ways to analyze the output,
[hotspot](https://github.com/KDAB/hotspot) might be the most convenient one at
the time of this writing.

## Build from the Source Code Repository with Conda Dependencies

To build these packages, you need a fairly recent C++ compiler and probably
some packages that might not be readily available on your system. If you don't
want to use your distribution's packages, you can provide these dependencies
with conda. Download and install [Mambaforge](https://github.com/conda-forge/miniforge#mambaforge),
then run

```
git clone --recurse-submodules https://github.com/flatsurf/flatsurf.git
cd flatsurf

mamba create -n flatsurf-build
mamba env update -n flatsurf-build -f libflatsurf/environment.yml
mamba env update -n flatsurf-build -f pyflatsurf/environment.yml
conda activate flatsurf-build

./bootstrap
./configure
make
```

Note that the C++ compiler package from conda disabled all assertions. To
enable assertions `export CPPFLAGS="$CPPFLAGS -UNDEBUG"` before running
configure and make.

For faster compile times, you might want to `mamba install ccache` and set
`export CXX="ccache c++"` before running configure and make.

## Build from the Source Code Repository with Conda

The conda recipes in `{lib,py}flatsurf/recipe/` are built automatically as part
of our Continuous Integration. If you want to build a recipe manually,
something like the following should work:

```
git clone --recurse-submodules https://github.com/flatsurf/flatsurf.git
cd flatsurf
conda activate root
conda config --add channels conda-forge
conda config --add channels flatsurf
conda install conda-build
conda build libflatsurf/recipe pyflatsurf/recipe
```

You can then try out the package that you just built with:
```
conda create -n flatsurf-test --use-local libflatsurf pyflatsurf
conda activate flatsurf-test
```

## Run Tests

`make check` runs all tests. During development `make check TESTS=module` only
runs the tests for `module`.

## Run Benchmarks

To build the benchmarks run `make` in `libflatsurf`. To run the
benchmarks, execute `libflatsurf/benchmark/benchmark`.

Alternatively, the benchmarks can be run through
[asv](https://asv.readthedocs.io/en/stable/index.html), e.g., with `asv dev`.

In VPATH builds, you can use `asv dev` by copying the `asv.conf.json` over to
the VPATH and setting `benchmark_dir` there to corresponding directory in the
original source tree. Then `ASV_PROJECT_DIR=``pwd``/libflatsurf MAKEFLAGS="-j4" asv dev`
should work as usual.

## How to Cite This Project

If you have used this project in the preparation of a publication, please cite
it as described [on our zenodo site](https://zenodo.org/record/4006152).

## Maintainers

* [@saraedum](https://github.com/saraedum)
* [@videlec](https://github.com/videlec)
