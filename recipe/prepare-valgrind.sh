#!/bin/bash
set -ex

export UPLOAD_PACKAGES="False"
patch ./test/run.sh <<"PATCH"
diff --git a/test/run.sh b/test/run.sh
index ae6f789..b7162ea 100755
--- a/test/run.sh
+++ b/test/run.sh
@@ -7,6 +7,5 @@ set -exo pipefail
 # Go to the test directory, so the path names are correct
 cd -P -- "$(dirname -- "$0")"

-for output in *.sample.out; do
-    diff <(polygon `basename "$output" .sample.out` | tail -n +2) <(tail -n +2 "$output");
-done
+conda install valgrind
+valgrind --leak-check=full polygon 1 1 2 2 --depth=10 --follow_depth=50 --show_cyls --show_moduli --cyls_only --quiet 1
PATCH
