**Deprecated:**

* Deprecated `Vertex::source(HalfEdge, FlatTriangulationCombinatorial&)` and `Vertex::target(HalfEdge, FlatTriangulationCombinatorial&)`. The parameter order everywhere else in flatsurf is such that the surface is passed first so prefer to use `Vertex::suorce(FlatTriangulationCombinatorial&, HalfEdge)` and `Vertex::target(FlatTriangulationCombinatorial&, HalfEdge)` instead.
