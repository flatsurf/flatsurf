#!/bin/bash
set -exo pipefail

if [ -z $PREFIX ]; then
        export PREFIX="$ASV_ENV_DIR"
fi

if [ -z $PREFIX ]; then
        echo '$PREFIX must be set. Aborting.'
        false
fi

# Create Makefiles
./bootstrap
# We need --disable-dependency-tracking so that a reinstall of host
# dependencies in the outputs section of the recipe does not trigger a rebuild
# of everything (since this updates the timestamps of the involved header
# files.)
./configure --prefix="$PREFIX" --disable-dependency-tracking CXXFLAGS="$CXXFLAGS" CXX=$CXX LDFLAGS="-L$PREFIX/lib -Wl,-rpath-link=$PREFIX/lib $LDFLAGS" CPPFLAGS="-isystem $PREFIX/include $CPPFLAGS" || (cat config.log; exit 1)
