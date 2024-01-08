# Catch2 Test Suite

We use [Catch2](https://github.com/catchorg/Catch2) to run a test suite.

If you're not familiar with Catch2, the `GENERATE` and `SECTION` macros might
be strange at first. For each `TEST_CASE`, Catch generates a tree of sections.
Each `SECTION` adds a new node under the current node. Each `GENERATE` adds a
node for each possible value. The entire test case is then run for each leaf in
the tree.

So, everything that happens in the `TEST_CASE` but outside of the generators,
happens for each run, i.e., essentially for the cartesian product of the
generators. Consider the following test

```cpp
const auto surface = GENERATE_SURFACES(T);

const auto something = expensive_computation(surface); // (*)

const auto saddle_connection = GENERATE_COPY(saddleConnections(surface));
```

The line marked with `*` is going to be executed for each saddle connection.
The following test is going to be much faster since it reuses the expensive
object, it's only computed once for each surface:

```cpp
const auto surface = GENERATE_SURFACES(T);

const auto something = GENERATE_COPY(value(expensive_computation(surface)));

const auto saddle_connection = GENERATE_COPY(saddleConnections(surface));
```
