#!/bin/bash
set -ex

source $RECIPE_DIR/environment.sh

cd pyflatsurf
$SNIPPETS_DIR/make/install.sh
