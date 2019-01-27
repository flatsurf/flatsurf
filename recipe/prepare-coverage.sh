#!/bin/bash
set -ex

patch ./.circleci/build_steps.sh <<"PATCH"
diff --git a/.circleci/build_steps.sh b/.circleci/build_steps.sh
index 1301eb6..415e94e 100755
--- a/.circleci/build_steps.sh
+++ b/.circleci/build_steps.sh
@@ -30,6 +30,7 @@ source run_conda_forge_build_setup
 make_build_number "${FEEDSTOCK_ROOT}" "${RECIPE_ROOT}" "${CONFIG_FILE}"
 
 conda build "${RECIPE_ROOT}" -m "${CI_SUPPORT}/${CONFIG}.yaml" \
+    --keep-old-work --no-remove-work-dir --old-build-string --dirty \
     --clobber-file "${CI_SUPPORT}/clobber_${CONFIG}.yaml"
 
 if [[ "${UPLOAD_PACKAGES}" != "False" ]]; then
PATCH

patch ./recipe/build.sh <<"PATCH"
diff --git a/recipe/build.sh b/recipe/build.sh
index 979e451..0c9c143 100644
--- a/recipe/build.sh
+++ b/recipe/build.sh
@@ -12,7 +12,7 @@ export CXXFLAGS="-O2 -g $CXXFLAGS"
 
 case `$CXX --version` in
     *GCC*|*gnu-c++*)
-        export EXTRA_CXXFLAGS="-Werror -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wundef -fdiagnostics-show-option -Wconversion"
+        export EXTRA_CXXFLAGS="--coverage"
         ;;
     *clang*)
         export EXTRA_CXXFLAGS="-Werror -Weverything -Wno-padded -Wno-exit-time-destructors -Wno-undefined-func-template -Wno-global-constructors -Wno-c++98-compat -Wno-missing-prototypes"
PATCH

patch ./test/run.sh <<"PATCH"
diff --git a/test/run.sh b/test/run.sh
index ae6f789..88087aa 100755
--- a/test/run.sh
+++ b/test/run.sh
@@ -10,3 +11,9 @@ cd -P -- "$(dirname -- "$0")"
 for output in *.sample.out; do
     diff <(polygon `basename "$output" .sample.out` | tail -n +2) <(tail -n +2 "$output");
 done
+
+conda install -y --quiet pip git gcc_linux-64
+pip install pyyaml cpp-coveralls
+cd $CONDA_BLD_PATH/polygon*/work
+set +x
+if [ -n "$COVERALLS_REPO_TOKEN" ];then coveralls --gcov `which x86_64-conda_cos6-linux-gnu-gcov` --gcov-options '\-lrp' -b src; fi
PATCH
