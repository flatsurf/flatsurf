**Added:**

* Added scalar multiplication of exact vectors with elements of underlying ring.
* Added `Point` for points on a flat triangulation.
* Added mapping of `Point` under a `Deformation`.
* Added `FlatTriangulation::angle` for `Point`.
* Added `FlatTriangulationCombinatorics::face(HalfEdge)` to get the other half edges of a face without having to manually call `previousInFace`/`nextInFace`.

**Performance:**

* Improved performance of tests.

