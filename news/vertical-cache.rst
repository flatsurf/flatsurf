**Changed:**

* `Vertical::self` is now "managed movable". This is a breaking ABI change but not an API change.

* Printing of `EdgeMap`, `HalfEdgeMap`, and `OddHalfEdgeMap` is now consistent.

**Performance:**

* Most methods of `Vertical` that take an `Edge` or a `HalfEdge` are now
  cached. This should speed up creating `ContourDecomposition` in many cases.

**Removed:**

* Removed deprecated method `FlatTriangulation::delaunay(HalfEdge)`, use
  `FlatTriangulation::delaunay(Edge)` instead.

* Removed deprecated method `FlatTriangulation::isomorphism(surface, predicate,
  predicate)`, use `FlatTriangulation::isomorphism(surface, ISOMORPHISM::FACES,
  predicate, predicate)` instead.

* Removed deprecated method
  `IntervalExchangeTransformation::makeUniqueLargeEdge()`, use
  `IntervalExchangeTransformation::makeUniqueLargeEdges` instead.

* Removed deprecated method `Vertical::horizontal()`, use
  `-Vertical::vertical().perpendicular()` instead.

* Removed deprecated method `Vertical::perpendicular(vector)`, use
  `Vertical::projectPerpendicular(vector)` instead.

* Removed deprecated method `Vertical::parallel(vector)`, use
  `Vertical::project(vector)` instead.

* Removed deprecated method `Vertical::parallel(HalfEdge)`, use
  `Vertical::ccw(HalfEdge) == CCW::COLLINEAR` instead.

* Removed depraceted method `Vertical::perpendicular(HalfEdge)`, use
  `Vertical::orientation(HalfEdge) == ORIENTATION::ORTHOGONAL` instead.
