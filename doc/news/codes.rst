**Added:**

* Added `Equivalence` and `EquivalenceClass` to quickly compare whether two surfaces are equal with respect to a certain notion of equality, e.g., modulo a relabeling of edges, modulo a linear transformation, …

* Added `FlatTriangulation::relabel()` to create a copy of the surface with permutated half edge labels.

**Changed:**

* <news item>

**Deprecated:**

* Deprecated `FlatTriangulation::isomorphism()`. For most purposes, `Equivalence::isomorphisms()` and `EquivalenceClass` are much faster and easier to use. The `isomorphism` can still be useful when considering non-triangulated surfaces. This functionality will be added to equivalences at a later point.

**Removed:**

* <news item>

**Fixed:**

* Fixed some compiler warnings with recent versions of GCC/Clang.

**Performance:**

* <news item>
