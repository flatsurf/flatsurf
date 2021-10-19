**Added:**

* Added `SaddleConnection::ccw()`.

* Added `SaddleConnectionsIterator::skip()` to explicitly backtrack in saddle connection search.

* Added `Path::tighten()`.

**Fixed:**

* A `Path` can now contain arbitrary turns. Before it was limited to the turn angles that show up in the perimeters of components.
