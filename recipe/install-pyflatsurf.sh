#!/bin/bash
cd pyflatsurf

# Run make install with the same flags that we use when invoking make
source $RECIPE_DIR/cflags.sh

make install CXXFLAGS="$CXXFLAGS $EXTRA_CXXFLAGS"
