export CXXFLAGS="-O2 -g $CXXFLAGS $EXTRA_CXXFLAGS"
export LD_LIBRARY_PATH="${PREFIX}/lib:${LD_LIBRARY_PATH}"

if [ "$(uname)" == "Darwin" ]; then
    CXXFLAGS="$CXXFLAGS -fno-common"
fi

./bootstrap
./configure --prefix="$PREFIX" CXXFLAGS="$CXXFLAGS" CXX=$CXX
make -j$CPU_COUNT
make install
