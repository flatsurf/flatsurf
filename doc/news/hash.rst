**Added:**

* Added hash functions for ``FlatTriangulationCombinatorial``, ``FlatTriangulation<T>``, and ``Permutation<T>``.

**Fixed:**

* Fixed comparison of surfaces defined over different rings. Surfaces defined over different rings are now never considered to be equal.

* Fixed ``trivial()`` for linear deformations. A linear deformation coming from the identity matrix is now only trivial if the domain and codomain are indistinguishable, i.e., if application of that matrix does not change the base ring.
