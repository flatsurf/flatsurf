**Added:**

* `FlatTriangulation::delaunay(Edge)` which returns a classification to distinguish ambiguous from non-ambiguous edges.

**Changed:**

* `FlatTriangulation::isomorphism` takes an extra parameter
  `ISOMORPHISM::FACES` or `ISOMORPHISM::DELAUNAY_CELLS` to only ask for
  isomorphisms of Delaunay cells, i.e., ignoring ambiguous edges that can be
  flipped without changing the Delaunay condition.

**Deprecated:**

* `FlatTriangulation::delaunay(HalfEdge)` as the Delaunay condition is not
  decided on the level of half edges but on the level of edges.

* `FlatTriangulation::isomorphism()` without the `ISOMORPHISM` parameter
  because one should be specific about the kind of isomorphism.
