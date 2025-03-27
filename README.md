<p align="center">
    <img alt="logo" src="https://avatars.githubusercontent.com/u/46691578?s=300&u=c65d11085307f34e1998cc996e6ee2d16b4d458a&v=4">
</p>

<h1><p align="center">libflatsurf & pyflatsurf</p></h1>

<p align="center">
  <img src="https://img.shields.io/badge/License-GPL_3.0_or_later-blue.svg" alt="License: GPL 3.0 or later">
  <a href="https://github.com/flatsurf/flatsurf/actions/workflows/test.yml"><img src="https://github.com/flatsurf/flatsurf/workflows/Test/badge.svg" alt="Test"></a>
  <a href="https://codecov.io/gh/flatsurf/flatsurf"><img src="https://codecov.io/gh/flatsurf/flatsurf/branch/master/graph/badge.svg" alt="Coverage"></a>
  <a href="https://flatsurf.github.io/flatsurf/asv/"><img src="http://img.shields.io/badge/benchmarked%20by-asv-blue.svg?style=flat" alt="Benchmarks"></a>
  <a href="https://doi.org/10.5281/zenodo.4006152"><img src="https://zenodo.org/badge/DOI/10.5281/zenodo.4006152.svg" alt="DOI 10.5281/zenodo.4006152"></a>
</p>

<p align="center">a C++ backend for sage-flatsurf</p>

libflatsurf is C++ library to work with [translation surfaces](https://en.wikipedia.org/wiki/Translation_surface) and pyflatsurf is a low-level wrapper to use libflatsurf from Python.

If you came here looking for the [**flatsurf project**](https://flatsurf.github.io), you probably want to start with [sage-flatsurf](https://github.com/flatsurf/sage-flatsurf) which provides a unified interface to these and many other libraries in the [SageMath](https://sagemath.org) computer algebra system.

Source tarballs can be downloaded at https://github.com/flatsurf/flatsurf/releases.

This repository contains two related projects:

* **libflatsurf** a C++ library
* **pyflatsurf** a Python wrapper for **libflatsurf**

The dependencies are:

 - [FLINT](https://flintlib.org)
 - [e-antic](https://github.com/flatsurf/e-antic)
 - [exact-real](https://github.com/flatsurf/exact-real)
 - [intervalxt](https://github.com/flatsurf/intervalxt)
 - [Boost](https://www.boost.org/)
 - [cppyy](https://cppyy.readthedocs.io/en/latest/) for the Python wrapper

## Usage

TODO

## Build and Develop e-antic with pixi

If you have cloned the source repository, make sure to pull in all the
third-party header-only libraries by running:

```
git submodule update --init
```

If you are distributing flatsurf or you are a purist who *only* wants to
interact with autotools directly, then you should skip to the [next
section](#build-from-the-source-code-repository-or-a-tarball). Otherwise, we
strongly recommend that you install [pixi](https://pixi.sh) and then use the
following commands:

* `pixi run test` build flatsurf and run the libflatsurf and pyflatsurf test suites
* `pixi run sage` build flatsurf and spawn SageMath with the local libflatsurf and pyflatsurf installed
* `pixi run doc` to build and preview the documentation
* `pixi run compile-commands` to generate a `compile_commands.json` that your IDE might be able to use to make sense of this project

<details>
<summary>What is pixi?</summary>

pixi is a tool based on
[conda](https://en.wikipedia.org/wiki/Conda_(package_manager)) &
[conda-forge](https://conda-forge.org) for developers so that we can all use
the same workflows in the same defined environments.

pixi allows us to ship a very opinionated setup to developers of flatsurf,
namely a number of opinionated scripts with corresponding tested (and
opinionated) dependencies.

This makes the whole development experience much more reliable and
reproducible, e.g., the CI on GitHub Pull Requests runs with the exact same
setup, so if something fails there, you can just run the CI command to
hopefully get exactly the same behavior locally.
</details>

<details>
<summary>How do I use pixi?</summary>

If you have not used pixi before, the most relevant pixi command is:

```sh
pixi run TASK
```

Run `pixi task list` to see the available tasks.

All tasks are defined in the `pixi.toml` file and most are used somewhere in
our GitHub Continuous Integration setup, see .github/workflows/.
</details>

<details>
<summary>Why don't we add all things to the Makefiles but use pixi tasks?</summary>

Packagers do prefer a system that is as minimalistic as possible. Any
opinionated bit in the build system, such as setting compiler flags, usually
needs to be patched out by software distributions. That's why our Makefiles are
trying to follow the autoconfiscated standards as closely as possible. And
essentially all that pixi does is to call these Makefiles without you having to
figure out how everything works in detail.
</details>

<details>
<summary>Can I use configure & make with pixi?</summary>

More experienced developers may not want to use these tasks. You can also just
use the curated list of dependencies that pixi provides and drop into a shell
with these dependencies installed. For example, to run the libflatsurf test suite
directly, you could do:

```sh
pixi shell -e dev
./bootstrap
cd libflatsurf
./configure
make check
```

Note that the following section contains more details about this `configure &&
make` workflow that might be of interest to you.
</details>

## Build from the Source Code Repository or a Tarball

If you have cloned the source directory and you decided not to use pixi, you
will need to setup the configure script and Makefile using autotools. That is

    git submodule update --init
    ./bootstrap

If you obtained a tarball of the sources or if the preceding step
worked, you can now run

    ./configure
    make
    make check  # to run our test suite
    make install  # to install into /usr/local

If you happen to have any dependency installed in a non standard directory, you
will have to specify the `CPPFLAGS` and `LDFLAGS` variables for the configure
script

    ./configure CPPFLAGS=-I/my/path/include LDFLAGS=-L/my/path/lib

For best performance run `CXXFLAGS="-O3 -flto -march=native -mtune=native"
CXX="g++ -fno-semantic-interposition" ./configure` instead of `./configure` as
this code greatly benefits from flto inlining. (Unfortunately, libtool filters
out `-fno-semantic-interposition` as of early 2019 so we can not pass it as
part of `CXXFLAGS`. If you are using clang, `-fno-semantic-interposition` does
not seem to be necessary.) Do not use `-Ofast` or `-ffast-math` as parts of our
code rely on IEEE compliance. You might want to add `-g3` to `CXXFLAGS` which
does not hurt performance but gives a better debugging experience. For the best
debugging experience, you might want to replace `-O3` with `-Og` or even `-O0`
but the latter results in poor performance.

Additionally, you might want to run configure with ` --disable-static`
which improves the build time.

If your compiler supports it, you can try to add `-fvisibility=hidden
-fvisibility-inlines-hidden` to your `CXXFLAGS`. This hides internal bits in
the resulting library which have lead to crashes in the past due to conflicting
header-only libraries.

<!--
If your linker supports it, you should use `./configure --with-version-script`
to shrink the resulting shared library to an exact curated list of versioned
symbols.
-->

perf works well to profile when you make sure that `CXXFLAGS`
contain `-fno-omit-framepointer`. You can then for example run our test suite
with `perf record --call-graph dwarf make check`. Apart from perf itself there
are several ways to analyze the output,
[hotspot](https://github.com/KDAB/hotspot) might be the most convenient one at
the time of this writing.

<!--
TODO: Make sure that we set UNDEBUG in our build!

Note that the C++ compiler package from conda disabled all assertions. To
enable assertions `export CPPFLAGS="$CPPFLAGS -UNDEBUG"` before running
configure and make.
-->

## Feedback and Contributions

If you have tried out flatsurf, we are thrilled to learn about your experiences.
If you ran into any problems or have suggestions for improvement, please
[create an issue](https://github.com/flatsurf/flatsurf/issues).

If you want to contribute to flatsurf, [pull
requests](https://github.com/flatsurf/flatsurf/pulls) are always welcome :heart:

We are also happy to walk you through the process personally if you are unsure
how to get started. Feel free to reach out in the [#flatsurf stream on
Zulip](https://sagemath.zulipchat.com/#narrow/channel/271193-flatsurf) in any
case.

## License

flatsurf is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License (GPL) as published by the Free Software
Foundation; either version 3.0 of the License, or (at your option) any later
version. See https://www.gnu.org/licenses/.

## How to Cite This Project

If you have used this project in the preparation of a publication, please cite
it as described [on our zenodo site](https://zenodo.org/record/4006152).

## Acknowledgements

* Julian Rüth's contributions to this project have been supported by the Simons
  Foundation Investigator grant of Alex Eskin.

## Maintainers

* [@saraedum](https://github.com/saraedum)
* [@videlec](https://github.com/videlec)
