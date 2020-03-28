#!/usr/bin/env python3
# -*- coding: utf-8 -*-

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

from pyflatsurf import flatsurf, Surface
import flatsurf as sage_flatsurf

@pytest.mark.parametrize("n,bound", [(4,3),(5,3),(6,3),(7,3)])
def test_veech_ngon(n, bound):
    S = sage_flatsurf.translation_surfaces.veech_2n_gon(n)
    S = Surface(S)
    for connection in S.saddle_connections(flatsurf.Bound(bound, 0)):
        v = connection.vector()
        decomposition = flatsurf.makeFlowDecomposition(S, v)
        flatsurf.decomposeFlowDecomposition(decomposition, -1)

        assert decomposition.completelyPeriodic() == True
        assert decomposition.parabolic() == True

        for dec in decomposition.components():
            h = dec.circumferenceHolonomy()
            assert h.x() or h.y()
            assert h.x() * v.y() == h.y() * v.x()

if __name__ == '__main__': sys.exit(pytest.main(sys.argv))
