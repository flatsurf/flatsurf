#!/bin/bash

# ##################################################################
# Source this file to setup additional environment variables for the
# conda-build environment.
# ##################################################################

export SNIPPETS_DIR=`realpath "${SNIPPETS_DIR:-$SRC_DIR/recipe/snippets}"`
export GITHUB_ORGANIZATION=flatsurf
export GITHUB_REPOSITORY=flatsurf

source $SNIPPETS_DIR/cxx/environment.sh
source $SNIPPETS_DIR/make/environment.sh
source $SNIPPETS_DIR/asv/environment.sh
source $SNIPPETS_DIR/codecov/environment.sh

export SUBDIRS=libflatsurf:pyflatsurf:flatsurf-polygon
