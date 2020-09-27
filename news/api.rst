**Added**

* Top level documentation for all classes in the public API.

**Changed:**

* Removed `std::unique_ptr` and `std::shared_ptr` from the public API. This is
  a breaking change that has changed large parts of the interface, see #214.

* `FlatTriangulation::fromEdge` is now called `fromHalfEdge`.

* `FlatTriangulation::slot` is now called `slit`.

* `FlatTriangulationCombinatorial::slot` is now called `slit`.

* `FlatTriangulation::fromEdgeApproximation` is now called `fromHalfEdgeApproximate`.

* `FlatTriangulationCollapsed::fromEdge` is now called `fromHalfEdge`.

* The serialization format (which we do not consider part of the stable API.)

* Moved Delaunay triangulation code into `FlatTriangulation` as a pair of
  `delaunay()` method.

**Removed:**

* Some workarounds for bugs in cppyy. These are not necessary with the latest
  cppyy 1.8.3 anymore.

**Fixed:**

* Fixed printing of some objects from Python.

* Implemented missing methods in `SaddleConnectionsByLength`.

* Removed some internal hacks such as the `Lengths::register` hack that was
  used to get weak pointers right.

* Simplified `noexcept` qualifications on all methods in the public API
