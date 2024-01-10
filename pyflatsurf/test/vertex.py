#!/usr/bin/env python

######################################################################
# This file is part of flatsurf.
#
#       Copyright (C) 2022 Julian Rüth
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

from pyflatsurf import flatsurf
import surfaces


def test_vertex_constructor():
    for coefficients in ['long long', 'mpz_class', 'mpq_class', 'eantic::renf_elem_class', 'exactreal::Element<exactreal::IntegerRing>', 'exactreal::Element<exactreal::RationalField>', 'exactreal::Element<exactreal::NumberField>']:
        surface = surfaces.square(flatsurf.Vector[coefficients])
        Surface = type(surface)

        # Annoyingly, we have to call .combinatorial() since the cast operator
        # from the FlatTriangulationCombinatorics base class is not seen by
        # cppyy it seems.
        v = flatsurf.Vertex.source(flatsurf.HalfEdge(1), surface.combinatorial())
        w = flatsurf.Vertex.source(flatsurf.HalfEdge(-1), surface.combinatorial())
        assert v == w


if __name__ == '__main__': sys.exit(pytest.main(sys.argv))

