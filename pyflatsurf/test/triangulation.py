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

import cppyy.gbl
from gmpxxyy import mpq
from pyflatsurf import flatsurf

def test_torus():
    verts = [cppyy.gbl.std.vector[int]([1,-3,2,-1,3,-2])]
    T = cppyy.gbl.flatsurf.FlatTriangulationCombinatorial(verts)
    assert T.size() == T.edges().size() == 3
    hols = [(1,0), (0,1), (-1,-1)]
    hols = cppyy.gbl.vector[flatsurf.Vector[mpq]]([flatsurf.Vector[mpq](x,y) for x,y in hols])
    F = cppyy.gbl.flatsurf.makeFlatTriangulation(T, hols)
    assert F.size() == F.edges().size() == 3

if __name__ == '__main__': sys.exit(pytest.main(sys.argv))

