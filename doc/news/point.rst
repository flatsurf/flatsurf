**Added:**

* Added scalar multiplication of exact vectors with elements of underlying ring.
* Added `Vector::applyMatrix(a, b, c, d)` and `FlatTriangulation::applyMatrix(a, b, c, d)`.
* Added predicates `FlatTriangulation::inHalfPlane()` and `FlatTriangulation::inPlane()`.
* Added various flavours of `FlatTriangulation::sector()` to generalize saddle
  connections factory functions that have been deprecated.
* Added `Point` for points on a flat triangulation.
* Added mapping of `Point` under a `Deformation`.
* Added `FlatTriangulation::angle` for `Point`.
* Added `FlatTriangulationCombinatorics::face(HalfEdge)` to get the other half edges of a face without having to manually call `previousInFace`/`nextInFace`.

**Fixed:**

* Fixed incorrect channels for binder setup.

**Performance:**

* Improved performance of tests.

**Deprecated:**

* Deprecated factory functions `SaddleConnection::inHalfPlane()`,
  `SaddleConnection::inPlane()`, `SaddleConnection::alongVertical()`,
  `SaddleConnection::clockwise()`, `SaddleConnection::counterclockwise()`. The
  functionality is now essentially in `FlatTriangulation::sector()`.

