**Added:**

* Added a copy constructor, a copy assignment operator, a move constructor, and
  a move assignment operator for a `Deformation`.
* Implemented special handling of a `Tracked<Deformation>` so that it can track
  flips in the tracked triangulation.
* Added glue to `flatsurf::Tracked` to pyflatsurf.
