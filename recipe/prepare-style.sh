#!/bin/bash
set -ex

export UPLOAD_PACKAGES="False"
patch ./recipe/build.sh <<"PATCH"
diff --git a/recipe/build.sh b/recipe/build.sh
index 1582523..d46516e 100644
--- a/recipe/build.sh
+++ b/recipe/build.sh
@@ -11,10 +11,8 @@ export CPPFLAGS="-isystem ${BUILD_PREFIX}/include $CPPFLAGS"
 export CXXFLAGS="-O2 -g $CXXFLAGS $EXTRA_CXXFLAGS"
 case `$CXX --version` in
     *GCC*|*gnu-c++*)
-        export WARN_CXXFLAGS="-Werror -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wundef -fdiagnostics-show-option -Wconversion"
         ;;
     *clang*)
-        export WARN_CXXFLAGS="-Werror -Weverything -Wno-padded -Wno-exit-time-destructors -Wno-undefined-func-template -Wno-global-constructors -Wno-c++98-compat -Wno-missing-prototypes"
         ;;
     *)
         $CXX --version
@@ -28,5 +26,6 @@ fi

 ./bootstrap
 ./configure --prefix="$PREFIX" CXXFLAGS="$CXXFLAGS" CXX=$CXX || (cat config.log; exit 1)
-make -j$CPU_COUNT CXXFLAGS="$CXXFLAGS $WARN_CXXFLAGS"
-make install
+conda install -y --quiet clang
+clang-format -i --style=Google src/*.cc src/libpolygon/*.h
+git diff --exit-code
PATCH
patch ./recipe/meta.yaml <<"PATCH"
diff --git a/recipe/meta.yaml b/recipe/meta.yaml
index f271f3d..6ba9c62 100644
--- a/recipe/meta.yaml
+++ b/recipe/meta.yaml
@@ -29,13 +29,13 @@ requirements:
 # Currently, conda-build does not run the test section when it is output-specific
 # Of course, this should go to the polygon output only. Likely this is going to work
 # again with a newer version of conda-build.
-test:
-  source_files:
-    - test/
-  commands:
-    - test/run.sh            # [linux]
-    # Tests on macOS fail at the moment. It's unclear whether that's a problem, see https://github.com/polygon-tbd/polygon/pull/5
-    - test/run.sh || true    # [osx]
+#test:
+#  source_files:
+#    - test/
+#  commands:
+#    - test/run.sh            # [linux]
+#    # Tests on macOS fail at the moment. It's unclear whether that's a problem, see https://github.com/polygon-tbd/polygon/pull/5
+#    - test/run.sh || true    # [osx]
 
 outputs:
   - name: libpolygon
PATCH
