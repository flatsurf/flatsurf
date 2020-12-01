**Changed:**

* `Vertical::self` is now "managed movable". This is a breaking ABI change but not an API change.

* Printing of `EdgeMap`, `HalfEdgeMap`, and `OddHalfEdgeMap` is now consistent.

**Performance:**

* Most methods of `Vertical` that take an `Edge` or a `HalfEdge` are now
  cached. This should speed up creating `ContourDecomposition` in many cases.
