export UPLOAD_PACKAGES="False"
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
