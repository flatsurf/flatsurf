#!/usr/bin/env python3
# -*- coding: utf-8 -*-

######################################################################
# This file is part of flatsurf.
#
#       Copyright (C) 2020 Julian Rüth
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

Edge = flatsurf.Edge

def test_addition():
    vector = flatsurf.Vector['mpq_class']

    L = surfaces.L(vector)

    # A stretched L
    shift = [vector(0, L.fromHalfEdge(e.positive()).y()) for e in L.edges()]
    L = (L + shift).surface()

    for halfEdge in L.halfEdges():
        assert L.fromHalfEdge(halfEdge).y() != 1


def test_deformation_square(capsys):
    vector = flatsurf.Vector['mpq_class']

    # A square of size (3, 3)
    square = surfaces.square(vector).scale(3)

    # Insert an extra vertex at (2, 1) that is easier to move around without
    # having to change the entire surface.
    square = square.insertAt(flatsurf.HalfEdge(1), vector(2, 1)).surface()

    # A forbidden shift, collapsing a half edge during the shift; this is
    # supposed to fail but it should fail silently.
    shift = lambda e: vector(-2, -4) if e in [Edge(4), Edge(5), Edge(6)] else vector(0, 0)

    capsys.readouterr()

    with pytest.raises(cppyy.gbl.std.invalid_argument):
        square + [shift(e) for e in square.edges()]

    # Verify that #179 has been fixed, i.e., no warnings are printed
    output = capsys.readouterr()
    assert output.out == ""
    assert output.err == ""

    # A much smaller shift in the same direction is allowed
    square + [shift(e) / 128 for e in square.edges()]

    # A shift half the length collapses the extra vertex
    square + [shift(e) / 2 for e in square.edges()]

    # Another shift with a vertex ending up on the interior of a half edge, i.e., requires a flip.
    shift = lambda e: vector(-1, 0) if e in [Edge(4), Edge(5), Edge(6)] else vector(0, 0)
    square + [shift(e) for e in square.edges()]

    # Another shift with a vertex crossing over the interior of a half edge, i.e., requires a flip.
    shift = lambda e: vector(-2, 0) if e in [Edge(4), Edge(5), Edge(6)] else vector(0, 0)
    square + [shift(e) for e in square.edges()]

    # Another shift with a vertex crossing over the interior of a half edge but
    # then ending up on another vertex, i.e., requires a flip and a collapse.
    shift = lambda e: vector(-4, -2) if e in [Edge(4), Edge(5), Edge(6)] else vector(0, 0)
    square + [shift(e) for e in square.edges()]

    # Another shift with a vertex crossing lots of half edges, i.e., requiring many flips.
    shift = lambda e: vector(-23, 0) if e in [Edge(4), Edge(5), Edge(6)] else vector(0, 0)
    square + [shift(e) for e in square.edges()]

    # Insert a vertex in the other triangle so we can move both simultaneously
    square = square.insertAt(flatsurf.HalfEdge(3), vector(1, 2)).surface()

    # Shift both inserted vertices towards the same vertex simultaneously
    shift = lambda e: vector(-1, -2) if e in [Edge(4), Edge(5), Edge(6)] else vector(-2, -1) if e in [Edge(7), Edge(8), Edge(9)] else vector(0, 0)
    square + [shift(e) / 2 for e in square.edges()]

    # Shift both inserted vertices onto the same vertex simultaneously
    square + [shift(e) for e in square.edges()]

    # Shift both inserted vertices a long way across the surface in the same direction
    shift = lambda e: vector(-23, 0) if e in [Edge(4), Edge(5), Edge(6)] else vector(-23, 0) if e in [Edge(7), Edge(8), Edge(9)] else vector(0, 0)
    square + [shift(e) for e in square.edges()]

    # Shift both inserted vertices a long way across the surface in different directions (invalid because it the two marked vertices meet at some point.)
    shift = lambda e: vector(-23, 0) if e in [Edge(4), Edge(5), Edge(6)] else vector(0, -23) if e in [Edge(7), Edge(8), Edge(9)] else vector(0, 0)
    with pytest.raises(cppyy.gbl.std.invalid_argument):
        square + [shift(e) for e in square.edges()]

    # Shift both inserted vertices a long way across the surface in different directions
    shift = lambda e: vector(-23, 0) if e in [Edge(4), Edge(5), Edge(6)] else vector(-1, -23) if e in [Edge(7), Edge(8), Edge(9)] else vector(0, 0)
    square + [shift(e) for e in square.edges()]

    # Shift both inserted vertices a long way across the surface in different directions and at different velocities
    shift = lambda e: vector(-23, 0) if e in [Edge(4), Edge(5), Edge(6)] else vector(0, -47) if e in [Edge(7), Edge(8), Edge(9)] else vector(0, 0)
    square + [shift(e) for e in square.edges()]

def test_deformation_L():
    vector = flatsurf.Vector['mpq_class']

    L = surfaces.L(vector)
    
    # Deform the L by doubling each vector
    L + [L.fromHalfEdge(e.positive()) for e in L.edges()]

def test_deform_hexagon():
    hexagon = surfaces.random_hexagon()

    # Deform the hexagon by doubling each vector
    hexagon + [hexagon.fromHalfEdge(e.positive()) for e in hexagon.edges()]

    # Deform the hexagon by adding another random hexagon
    hexagon_ = surfaces.random_hexagon()
    hexagon + [hexagon_.fromHalfEdge(e.positive()) for e in hexagon.edges()]

def test_isomorphism():
    vector = flatsurf.Vector['mpq_class']
    L = surfaces.L(vector)
    assert L.isomorphism(L)
    assert L.isomorphism(L, filter_matrix=lambda a, b, c, d: a == -1 and b == 0 and c == 0 and d == -1)
    assert not L.isomorphism(L, filter_matrix=lambda a, b, c, d: a*d - b*c not in [-1, 1])

def test_serialization():
    hexagon = surfaces.random_hexagon()

    from pickle import loads, dumps
    assert loads(dumps(hexagon)) == hexagon

if __name__ == '__main__': sys.exit(pytest.main(sys.argv))
