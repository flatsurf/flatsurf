export CXXFLAGS="$CXXFLAGS -g3"

autoreconf -ivf
./configure --prefix="$PREFIX" --without-benchmark
make install
