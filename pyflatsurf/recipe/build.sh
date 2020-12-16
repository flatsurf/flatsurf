set -exo pipefail

cd pyflatsurf

autoreconf -ivf
./configure --prefix="$PREFIX" --without-pytest --without-sage
make install
