#!/bin/bash
set -ex

patch ./recipe/build.sh <<"PATCH"
diff --git a/recipe/build.sh b/recipe/build.sh
index 979e451..bf7b394 100644
--- a/recipe/build.sh
+++ b/recipe/build.sh
@@ -31,5 +31,12 @@ esac

 ./bootstrap
 ./configure --prefix="$PREFIX" CXXFLAGS="$CXXFLAGS" CXX=$CXX || (cat config.log; exit 1)
-make -j$CPU_COUNT CXXFLAGS="$CXXFLAGS $EXTRA_CXXFLAGS"
-make install
+conda install -y --quiet clangdev cppcheck
+cppcheck --enable=all --error-exitcode=1 --inline-suppr -j$CPU_COUNT src
+
+clang-format -i --style=Google src/*.cc src/libpolygon/*.h
+git diff --exit-code
+
+make -j$CPU_COUNT distcheck
PATCH

# Disable other testing
echo > test/run.sh
