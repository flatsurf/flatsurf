**Changed:**

* methods that modify a `FlatTriangulation` now return a `Deformation`.
  Currently that `Deformation` only wraps a `FlatTriangulation` but eventually
  we'd like to add information on how say vertices in the deformed surface
  relate to vertices in the original surface and such.
