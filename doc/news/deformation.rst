**Added:**

* Added actual functionality to `Deformation` class. Before it was just a
  placeholder except for completely trivial cases.

* More overloads of `SaddleConnection::inPlane` and `SaddleConnection::counterclockwise`.

* More convenience methods to `Path` such as `pop_front`, `pop_back`, and an
  implicit constructor from `SaddleConnection`.

* A conversion operator `Tracked<T>::operator T&&()` to move out of a `Tracked<T>`.

* An operator ~ on combinatorial triangulations that "inverts" a triangulation,
  i.e., it returns its mirror image. This is used, e.g., when applying a linear
  transformation of negative determinant to a triangulation.

**Changed:**

* `SaddleConnection::counterclockwise` now does what the documentation claimed.
  Before it actually implemented `SaddleConnection::inPlane`.

* Shortened printing of saddle connections when they coincide with a half edge,
  i.e., "2" instead of "(x, y) from 2 to -2".

**Deprecated:**

* Copy constructor and copy assignment for `Tracked<T>`. Many `T` do not
  support this and the registered callbacks might not support such copying so
  it is better not to support this use case.

**Removed:**

* <news item>

**Fixed:**

* Make `HalfEdgeMap` work for types that are not printable.

* Implemented `SaddleConnection::angle`. Technically, this is a breaking API
  change since the return type changed. However, before this method was not
  implemented at all so nobody could have relied on that.

**Performance:**

* <news item>
