#!/bin/bash
set -ex

export UPLOAD_PACKAGES="False"
patch ./recipe/build.sh <<"PATCH"
diff --git a/recipe/build.sh b/recipe/build.sh
index 979e451..f667099 100644
--- a/recipe/build.sh
+++ b/recipe/build.sh
@@ -31,5 +31,5 @@ esac

 ./bootstrap
 ./configure --prefix="$PREFIX" CXXFLAGS="$CXXFLAGS" CXX=$CXX || (cat config.log; exit 1)
-make -j$CPU_COUNT CXXFLAGS="$CXXFLAGS $EXTRA_CXXFLAGS"
-make install
+conda install -y --quiet cppcheck
+cppcheck --enable=all --error-exitcode=1 --inline-suppr -j$CPU_COUNT src
PATCH

# Disable other testing
echo > ./test/run.sh

git config --global user.email nobody@circleci.com
git config --global user.name CircleCI
git commit -am "temporary changes for this flavour"
