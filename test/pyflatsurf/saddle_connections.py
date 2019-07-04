#!/usr/bin/env python
# -*- coding: utf-8 -*-

######################################################################
#  This file is part of flatsurf.
#
#        Copyright (C) 2019 Julian Rüth
#
#  flatsurf is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  flatsurf is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with flatsurf. If not, see <https://www.gnu.org/licenses/>.
######################################################################

import sys
import pytest
import ctypes

from pyflatsurf import flatsurf
import surfaces

def test_square_longlong():
    surface = surfaces.square(flatsurf.Vector['long long'])
    connections = surface.saddleConnections(flatsurf.Bound(16), flatsurf.HalfEdge(1))
    # https://bitbucket.org/wlav/cppyy/issues/103/std-distance-returns-random-output
    # assert len(connections) == 60
    # This does not work, the iterator that backs connections returns
    # references that are only valid until the iterator is advanced, so the
    # conversion to list does not work at the moment.
    # assert len(list(connections))
    assert len([1 for c in connections]) == 60

def test_hexagon_eantic():
    surface = surfaces.hexagon()
    connections = surface.saddleConnections(flatsurf.Bound(16), flatsurf.HalfEdge(1))
    assert len([1 for c in connections]) == 10

def test_hexagon_exactreal():
    from pyexactreal import exactreal
    surface = surfaces.random_hexagon()
    connections = surface.saddleConnections(flatsurf.Bound(16), flatsurf.HalfEdge(1))
    # This is very slow at the moment, see https://github.com/flatsurf/exact-real/issues/37
    # assert len([1 for c in connections]) >= 10

if __name__ == '__main__': sys.exit(pytest.main(sys.argv))
