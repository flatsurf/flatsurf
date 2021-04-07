**Added:**

* Added `SaddleConnection::path()` that returns a more detailed path of a
  saddle connection than did `SaddleConnection::crossings()`. This can be used
  to properly shade components of a flow decomposition when drawing them.

**Deprecated:**

* The method `SaddleConnection::crossings()` has been superseded by
  `SaddleConnection::path()`.
