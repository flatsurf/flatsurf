#!/bin/bash
set -ex

# Run ./configure
source $RECIPE_DIR/cflags.sh
$RECIPE_DIR/configure.sh

make CXXFLAGS="$CXXFLAGS $EXTRA_CXXFLAGS"

# Run all our test suites
make check CXXFLAGS="$CXXFLAGS $EXTRA_CXXFLAGS"
if [[ "$build_flavour" == "release" ]]; then
    pushd libflatsurf
    make check-valgrind CXXFLAGS="$CXXFLAGS $EXTRA_CXXFLAGS"
    make distcheck
    popd
    pushd pyflatsurf
    make check-valgrind CXXFLAGS="$CXXFLAGS $EXTRA_CXXFLAGS"
    # Check would fail since libflatsurf is not installed
    # make distcheck
    popd
    pushd flatsurf-polygon
    make check-valgrind CXXFLAGS="$CXXFLAGS $EXTRA_CXXFLAGS"
    # Check would fail since libflatsurf is not installed
    # make distcheck
    popd

    make distcheck
fi

# /tmp/secrets contains some CI-injected credentials to services such as
# codecov or ASV.
source /tmp/secrets || true

if [[ "$build_flavour" == "release" ]]; then
    # Run benchmarks on the release variant
    $RECIPE_DIR/run-asv.sh

    # Enforce proper formatting of C++ code
    clang-format -i -style=file `git ls-files '*.cc' '*.hpp' '*.ipp' '*.h' '*.h.in' '*.hpp.in'`
    # Ignore submodules
    git submodule foreach git clean -fd
    git submodule foreach git reset --hard
    git diff --exit-code

    # Make sure there's no pending todos
    ! grep "TO""DO" `git ls-files | grep -v external | grep -v azure-pipelines`
else
    # Send coverage data to coveralls.io
    $RECIPE_DIR/coverage.sh
fi
