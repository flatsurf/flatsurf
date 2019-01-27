#!/bin/bash
set -ex

patch ./recipe/build.sh <<"PATCH"
diff --git a/recipe/build.sh b/recipe/build.sh
index 979e451..83b40cd 100644
--- a/recipe/build.sh
+++ b/recipe/build.sh
@@ -13,6 +13,8 @@ export CXXFLAGS="-O2 -g $CXXFLAGS"
 case `$CXX --version` in
     *GCC*|*gnu-c++*)
         export EXTRA_CXXFLAGS="-Werror -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wundef -fdiagnostics-show-option -Wconversion"
+        # Instrument code to detect errors at runtime
+        export EXTRA_CXXFLAGS="-fsanitize=address -fsanitize=leak -fsanitize=undefined -static-libasan -static-liblsan -static-libubsan"
         ;;
     *clang*)
         export EXTRA_CXXFLAGS="-Werror -Weverything -Wno-padded -Wno-exit-time-destructors -Wno-undefined-func-template -Wno-global-constructors -Wno-c++98-compat -Wno-missing-prototypes"
PATCH

patch ./test/run.sh <<"PATCH"
diff --git a/test/run.sh b/test/run.sh
index ae6f789..9a545a4 100755
--- a/test/run.sh
+++ b/test/run.sh
@@ -7,6 +7,13 @@ set -exo pipefail
 # Go to the test directory, so the path names are correct
 cd -P -- "$(dirname -- "$0")"
 
+# Run a simple case with valgrind to detect memory leaks (eventually, we should
+# run all tests but with a timeout.)
+conda install -y --quiet valgrind
+valgrind --leak-check=full polygon 1 1 2 2 --depth=10 --follow_depth=50 --show_cyls --show_moduli --cyls_only --quiet
+
+# Run other tests with less expensive instrumentation
+export ASAN_OPTIONS="detect_invalid_pointer_pairs=2"
 for output in *.sample.out; do
     diff <(polygon `basename "$output" .sample.out` | tail -n +2) <(tail -n +2 "$output");
 done
PATCH

git config --global user.email nobody@circleci.com
git config --global user.name CircleCI
git commit -am "temporary changes for this flavour"
