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
import ctypes

from sage.all import QQ
from pyflatsurf import flatsurf, Surface
import flatsurf as sage_flatsurf

def test_D33():
    S = sage_flatsurf.translation_surfaces.mcmullen_genus2_prototype(4,2,1,1,QQ((1,4)))
    S = Surface(S)
    for connection in S.saddle_connections(flatsurf.Bound(4, 0)):
        v = connection.vector()
        decomposition = flatsurf.makeFlowDecomposition(S, v)
        flatsurf.decomposeFlowDecomposition(decomposition, -1)
        ncyl, nmin, nund = 0, 0, 0
        for dec in decomposition.components():
            if dec.cylinder() == True:
                ncyl += 1
                h = dec.circumferenceHolonomy()
                assert h.x() or h.y()
                assert h.x() * v.y() == h.y() * v.x()
            elif dec.cylinder() == False:
                nmin += 1
            else:
                nund = 0
        assert nund == 0
        assert ((2 <= ncyl <= 3 and nmin == 0 ) or (ncyl == 0 and nmin == 2))

if __name__ == '__main__': sys.exit(pytest.main(sys.argv))
