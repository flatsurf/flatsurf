**Fixed:**

* Do not prune decomposition as soon as a component reached the induction limit
  in a `FlowDecomposition::decompose()`. Such a pruning would only affect the
  local subtree and not the entire decomposition. There is (and was) no way to
  stop the entire decomposition once a limit has been reached.
