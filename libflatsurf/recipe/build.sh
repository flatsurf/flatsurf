export CXXFLAGS="$CXXFLAGS -g3 -UNDEBUG"

cd libflatsurf

autoreconf -ivf
./configure --prefix="$PREFIX" --without-benchmark --disable-static || (cat config.log && exit 1)
make install
