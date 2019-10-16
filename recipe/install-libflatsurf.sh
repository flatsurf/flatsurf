#!/bin/bash
set -ex

source $RECIPE_DIR/environment.sh

cd libflatsurf
$SNIPPETS_DIR/make/install.sh
