unset BINSTAR_TOKEN
patch ./recipe/build.sh <<"PATCH"
diff --git a/recipe/build.sh b/recipe/build.sh
index f6c029e..78de1df 100644
--- a/recipe/build.sh
+++ b/recipe/build.sh
@@ -9,5 +9,4 @@ fi

 ./bootstrap
 ./configure --prefix="$PREFIX" CXXFLAGS="$CXXFLAGS" CXX=$CXX || (cat config.log; exit 1)
-make -j$CPU_COUNT
-make install
+make -j$CPU_COUNT distcheck
PATCH
