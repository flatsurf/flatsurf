External Header-Only Libraries
==============================

We rely on some header-only libraries that we vendor (as is customary) with our
source code.

* [rx-ranges](https://github.com/simonask/rx-ranges), provides features similar
  to the proposed [range-v3](https://github.com/ericniebler/range-v3) but
  without the (as of 2019) prohibitive increase in compilation times.
* [gmpxxll](https://github.com/flatsurf/gmpxxll), provides initialization of
  GMP's C++ wrapper from the long long data type.
