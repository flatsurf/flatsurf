**Performance:**

* Improved performance of `ContourDecompsoition` constructor by removing
  redundant assertion. In flow decompositions that are trivial to compute, this
  assertion often dominated the overall runtime.
