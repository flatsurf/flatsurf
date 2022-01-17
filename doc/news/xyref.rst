**Performance:**

* `Vector::x()` and `Vector::y()` now return a `const T&` instead of a `T`. This is a breaking ABI change that speeds up a some algorithms on surface with expensive coefficients (exact-real) by a few percent.
