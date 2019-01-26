set -ex
shopt -s extglob
export CPPFLAGS="-I${BUILD_PREFIX}/include $CPPFLAGS"
export CXXFLAGS="-O2 -g $CXXFLAGS $EXTRA_CXXFLAGS"
case `$CXX --version` in
    *GCC*|*gnu-c++*)
        export WARN_CXXFLAGS="-Werror -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wundef -fdiagnostics-show-option -Wconversion"
        ;;
    *clang*)
        export WARN_CXXFLAGS="-Werror -Weverything -Wno-padded -Wno-exit-time-destructors -Wno-undefined-func-template -Wno-global-constructors -Wno-c++98-compat -Wno-missing-prototypes"
        ;;
    *)
        $CXX --version
        echo "Which compiler is this?"
        false;
esac

if [ "$(uname)" == "Darwin" ]; then
    export CXXFLAGS="$CXXFLAGS -fno-common"
fi

./bootstrap
./configure --prefix="$PREFIX" CXXFLAGS="$CXXFLAGS" CXX=$CXX || (cat config.log; exit 1)
make -j$CPU_COUNT CXXFLAGS="$CXXFLAGS $WARN_CXXFLAGS"
make install
