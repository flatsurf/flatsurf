#!/bin/bash
set -ex

source $RECIPE_DIR/environment.sh

cd flatsurf-polygon
$SNIPPETS_DIR/make/install.sh
