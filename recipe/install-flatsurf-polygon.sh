#!/bin/bash
cd flatsurf-polygon

# Run make install with the same flags that we use when invoking make
source $RECIPE_DIR/cflags.sh

make -d install CXXFLAGS="$CXXFLAGS $EXTRA_CXXFLAGS"
