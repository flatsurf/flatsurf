#!/usr/bin/env python3
# -*- coding: utf-8 -*-
r"""
Eskin-McMullen-Mukamel-Write quadrilaterals from the article
"Billiards, quadrilaterals and moduli spaces"

Note that in some cases, the billiards might give a Veech surface and not
the ambient rank 2 locus.
"""
######################################################################
# This file is part of flatsurf.
#
#       Copyright (C) 2020 Vincent Delecroix
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

import itertools

from sage.all import AA
import flatsurf as sage_flatsurf
from pyflatsurf import GL2ROrbitClosure

@pytest.mark.parametrize("a,b,c,d,l1,l2,veech", [
    (1,1,1,7,1,1,True),
    (1,1,1,7,AA(2).sqrt(),1,False),
    (1,1,1,9,1,1,True),
    (1,1,1,9,AA(2).sqrt(),1,False),
    (2,2,3,13,1,1,False),
    (1,1,2,8,1,1,False),
    (1,1,2,12,1,1,False),
    (1,2,2,11,1,1,False),
    (1,2,2,15,1,1,False)
])
def test_rank2_quadrilateral(a, b, c, d, l1, l2, veech):
    E = sage_flatsurf.EquiangularPolygons(a, b, c, d)
    P = E([l1, l2])
    B = sage_flatsurf.similarity_surfaces.billiard(P)
    S = B.minimal_cover(cover_type="translation")
    S = S.erase_marked_points()
    S, _ = S.normalized_coordinates()
    O = GL2ROrbitClosure(S)
    D = itertools.islice(O.decompositions(9, 100), 50)
    if veech:
        assert S.base_ring().degree() <= 2
        for dec in D:
            O.update_tangent_space_from_flow_decomposition(dec)
            assert dec.parabolic()
            assert O.U.dimension() == 2, (O.U.dimension(), O.absolute_dimension())
    else:
        for dec in D:
            O.update_tangent_space_from_flow_decomposition(dec)
        assert O.absolute_dimension() == O.U.dimension() == 4, (O.U.dimension(), O.absolute_dimension())

if __name__ == '__main__': sys.exit(pytest.main(sys.argv))
