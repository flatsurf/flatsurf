[![Linux](https://img.shields.io/circleci/project/github/polygon-tbd/polygon/master.svg?label=Linux)](https://circleci.com/gh/polygon-tbd/polygon)
[![OSX](https://img.shields.io/travis/polygon-tbd/polygon/master.svg?label=macOS)](https://travis-ci.org/polygon-tbd/polygon)
![Windows disabled](https://img.shields.io/badge/Windows-disabled-lightgrey.svg)
[![Coverage Status](https://coveralls.io/repos/github/polygon-tbd/polygon/badge.svg?branch=master)](https://coveralls.io/github/polygon-tbd/polygon?branch=master)

## Current Release Info

We build and release this package with every push to the master branch. These releases are considered unstable and highly
experimental. There are no stable releases yet.

| Name | Downloads | Version | Platforms |
| --- | --- | --- | --- |
| [![Nightly Build](https://img.shields.io/badge/experimental-polygon-green.svg)](https://anaconda.org/saraedum/polygon) | [![Conda Downloads](https://img.shields.io/conda/dn/saraedum/polygon.svg)](https://anaconda.org/saraedum/polygon) | [![Conda Version](https://img.shields.io/conda/vn/saraedum/polygon.svg)](https://anaconda.org/saraedum/polygon) | [![Conda Platforms](https://img.shields.io/conda/pn/saraedum/polygon.svg)](https://anaconda.org/saraedum/polygon) |

## Install with Conda

You can install this package with conda. Download and install [Miniconda](https://conda.io/miniconda.html), then run

```
conda config --add channels conda-forge
conda install -c saraedum polygon
```

## Build from the Source Code Repository

We are following a standard autoconf setup, i.e., you can create the executable `src/polygon` with the following:

```
git clone https://github.com/polygon-tbd/polygon.git
cd polygon
./bootstrap
./configure
make
```
## Maintainers

* [@saraedum](https://github.com/saraedum)
