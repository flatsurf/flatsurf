Generic Algorithms for Vectors
==============================

The files in this directory implement generically algorithms defined in the
corresponding detail/ header files. These implementations always look for a
customization point in the Implementation and otherwise fall back to a generic
algorithm that depends on the chosen storage.

We realize the customization points through the *detection idiom* until
Concepts are widely supported. Since that idiom is only part of
`<experimental/type_traits>`, we ship a version from
[cppreference](https://en.cppreference.com/w/cpp/experimental/is_detected).
That version is CC-BY-SA 3.0 licensed and can therefore be relicensed as GPL3.
(Technically, not as GPL3+, since it's not clear whether future version will
be compatible with CC-BY-SA 4.0 but that's a theoretical problem with
virtually any license that is not GPLx+.
