set -ex
export CPPFLAGS="-I${BUILD_PREFIX}/include $CPPFLAGS"
export CXXFLAGS="-O2 -g $CXXFLAGS $EXTRA_CXXFLAGS"
export LD_LIBRARY_PATH="${PREFIX}/lib:${LD_LIBRARY_PATH}"

if [ "$(uname)" == "Darwin" ]; then
    export CXXFLAGS="$CXXFLAGS -fno-common"
fi

./bootstrap
./configure --prefix="$PREFIX" CXXFLAGS="$CXXFLAGS" CXX=$CXX || (cat config.log; exit 1)
make -j$CPU_COUNT
make install
