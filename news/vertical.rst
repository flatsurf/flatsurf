**Added:**

* several methods in `Vertical`, mostly replicating existing functionality but with more intuitive naming.

**Deprecated:**

* several methods in `Vertical`, namely some flavours of `parallel`, `perpendicular`, and `horizontal`.

**Performance:**

* with exact-real vectors, not actually performing that many exact scalar
  multiplication in `Vertical` might make a difference in some instances.
