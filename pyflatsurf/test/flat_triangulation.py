#!/usr/bin/env python

######################################################################
# This file is part of flatsurf.
#
#       Copyright (C) 2020-2025 Julian Rüth
#
# flatsurf is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# flatsurf is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with flatsurf. If not, see <https://www.gnu.org/licenses/>.
######################################################################

import sys
import pytest
import cppyy

from pyflatsurf import flatsurf
import surfaces

def test_addition():
    vector = flatsurf.Vector['mpq_class']

    L = surfaces.L(vector)

    # A stretched L
    shift = [vector(0, L.fromHalfEdge(e.positive()).y()) for e in L.edges()]
    L = (L + shift).surface()

    for halfEdge in L.halfEdges():
        assert L.fromHalfEdge(halfEdge).y() != 1

def test_isomorphism():
    vector = flatsurf.Vector['mpq_class']
    L = surfaces.L(vector)
    assert L.isomorphism(L)
    assert L.isomorphism(L, filter_matrix=lambda a, b, c, d: a == -1 and b == 0 and c == 0 and d == -1)
    assert not L.isomorphism(L, filter_matrix=lambda a, b, c, d: a*d - b*c not in [-1, 1])

def test_serialization():
    hexagon = surfaces.random_hexagon(flatsurf.Vector['exactreal::Element<exactreal::NumberField>'])

    from pickle import loads, dumps
    clone = loads(dumps(hexagon))
    assert clone == hexagon
    assert hash(clone) == hash(hexagon)

def test_readme():
    # Make sure to update the README.md if you make any changes to this test case.
    import pyflatsurf
    import cppyy
    import pyeantic

    K = pyeantic.eantic.renf_class.make("x^2 - 3", "x", "1.73 +/- 0.1")
    x = K.gen()
    R2 = pyflatsurf.flatsurf.Vector[cppyy.gbl.eantic.renf_elem_class]
    R = pyeantic.eantic.renf_elem
    vectors = [R2(R(K, 2), R(K, 0)), R2(R(K, 1), x), R2(R(K, 3), x), R2(R(K, 1), -x), R2(R(K, 4), R(K, 0)), R2(R(K, 3), x)]
    vertices = [[1, 3, -4, -5, -3, -2], [2, -1, -6, 4, 5, 6]]
    S = pyflatsurf.Surface(vertices, vectors)

    C = S.connections().bound(4)
    assert "\n".join(str(c) for c in C) == "1\n-1\n2\n(-3, (x ~ 1.7320508)) from 2 to 4\n(0, (2*x ~ 3.4641016)) from 2 to -6\n(-2, (2*x ~ 3.4641016)) from 2 to -2\n-2\n(3, (-x ~ -1.7320508)) from -2 to -4\n(0, (-2*x ~ -3.4641016)) from -2 to 3\n(2, (-2*x ~ -3.4641016)) from -2 to 2\n3\n(2, (2*x ~ 3.4641016)) from 3 to -6\n(0, (2*x ~ 3.4641016)) from 3 to -2\n-3\n4\n(3, (-x ~ -1.7320508)) from 4 to 2\n-4\n(-3, (x ~ 1.7320508)) from -4 to -2\n5\n-5\n6\n-6\n(-2, (-2*x ~ -3.4641016)) from -6 to 3\n(0, (-2*x ~ -3.4641016)) from -6 to 2"


if __name__ == '__main__': sys.exit(pytest.main(sys.argv))
