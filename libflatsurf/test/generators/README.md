Catch2 has a concept called
[generators](https://github.com/catchorg/Catch2/blob/master/docs/generators.md)
which are essentially fancy loops.

It is important not to put SECTION or similar inside a for loop but to use
generators instead. Otherwise, the performance impact can be very significant,
see https://github.com/catchorg/Catch2/issues/993#issuecomment-336457580.

Therefore, we built some generators for the objects we most commonly want to
loop over.
