export CXXFLAGS="$CXXFLAGS -g3 -UNDEBUG"

cd libflatsurf

autoreconf -ivf
./configure --prefix="$PREFIX" --without-benchmark --disable-static
make install
