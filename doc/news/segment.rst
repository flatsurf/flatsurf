**Added:**

* Added `Ray` to describe a direction at a point; this unifies some functionality from `SaddleConnection` and `Vertical`.

* Added `Vector::parallel` as a shortcut for the repetetive `Vector::ccw() == COLLINEAR && Vector::orientation() == SAME`.

* Added `Segment` generalizing `SaddleConnection`.

* Added possibility to create `Path` from `Segment` or a sequence of `Segment`
  (before paths could only be built from saddle connections.)

# Added `Chain::vector()` method to make it easier to cast a chain to vector in Python.

**Changed:**

* <news item>

**Deprecated:**

* <news item>

**Removed:**

* <news item>

**Fixed:**

* Fixed hashing of `SaddleConnection`. Before, two saddle connections with different underlying representations as chains that represented the same vector were considered different from each other.
* <news item>

**Performance:**

* <news item>
