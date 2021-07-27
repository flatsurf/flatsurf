**Fixed:**

* Changed ordering produced by `flatsurf::detail::VectorExact<>::CompareSlope`
  so it identifies infinite slopes, i.e., `(0, 1)` and `(0, -1)`.
