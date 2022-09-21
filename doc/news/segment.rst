**Added:**

* Added `Ray` to describe a direction at a point; this unifies some functionality from `SaddleConnection` and `Vertical`.

* Added `Vector::parallel` as a shortcut for the repetetive `Vector::ccw() == COLLINEAR && Vector::orientation() == SAME`.

* Added `Segment` generalizing `SaddleConnection`.

* Added possibility to create `Path` from `Segment` or a sequence of `Segment`
  (before paths could only be built from saddle connections.)

**Changed:**

* <news item>

**Deprecated:**

* <news item>

**Removed:**

* <news item>

**Fixed:**

* <news item>

**Performance:**

* <news item>
