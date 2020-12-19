export CXXFLAGS="$CXXFLAGS -g3 -UNDEBUG"

autoreconf -ivf
./configure --prefix="$PREFIX" --without-benchmark
make install
