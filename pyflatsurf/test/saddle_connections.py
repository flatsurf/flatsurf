#!/usr/bin/env python3
# -*- coding: utf-8 -*-

######################################################################
# This file is part of flatsurf.
#
#       Copyright (C) 2019 Julian Rüth
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

from pyflatsurf import flatsurf
import surfaces

def test_square_longlong():
    surface = surfaces.square(flatsurf.Vector['long long'])
    connections = surface.saddle_connections(flatsurf.Bound(16, 0), flatsurf.HalfEdge(1))
    # https://bitbucket.org/wlav/cppyy/issues/103/std-distance-returns-random-output
    # assert len(connections) == 60
    # This does not work, the iterator that backs connections returns
    # references that are only valid until the iterator is advanced, so the
    # conversion to list does not work at the moment.
    # assert len(list(connections))
    assert len([1 for c in connections]) == 60

def test_L_mpq():
    surface = surfaces.L(flatsurf.Vector['mpq_class'])
    connections = surface.saddle_connections(flatsurf.Bound(16, 0), flatsurf.HalfEdge(1))
    assert len([1 for c in connections]) == 60

def test_L_with_slit_mpq():
    import cppyy

    mpq = cppyy.gbl.mpq_class
    R2 = flatsurf.Vector[mpq]
    surface = surfaces.L(R2)
    slit = R2(mpq(5, 3), mpq(4, 3))
    e = flatsurf.HalfEdge(1)
    surface = surface.insertAt(e, slit)
    assert e != flatsurf.HalfEdge(1), "HalfEdge& not updated correctly in " + repr(surface)
    e = surface.nextAtVertex(e)
    surface = cppyy.gbl.flatsurf.slot(surface, e)

    connections = surface.saddle_connections(flatsurf.Bound(16, 0), flatsurf.HalfEdge(1))
    assert len([1 for c in connections]) == 15

def test_hexagon_eantic():
    surface = surfaces.hexagon()
    connections = surface.saddle_connections(flatsurf.Bound(16, 0), flatsurf.HalfEdge(1))
    assert len([1 for c in connections]) == 10

def test_hexagon_exactreal():
    from pyexactreal import exactreal
    surface = surfaces.random_hexagon()
    connections = surface.saddle_connections(flatsurf.Bound(16, 0), flatsurf.HalfEdge(1))
    # This is very slow at the moment, see https://github.com/flatsurf/exact-real/issues/37
    # assert len([1 for c in connections]) >= 10

if __name__ == '__main__': sys.exit(pytest.main(sys.argv))
