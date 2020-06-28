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

def test_addition():
    vector = flatsurf.Vector['mpq_class']

    L = surfaces.L(vector)

    # A stretched L
    shift = [vector(0, L.fromEdge(e.positive()).y()) for e in L.edges()]
    L += shift

    for halfEdge in L.halfEdges():
        assert L.fromEdge(halfEdge).y() != 1


def test_deformation(capsys):
    vector = flatsurf.Vector['mpq_class']

    # A square of size (3, 3)
    square = surfaces.square(vector).scale(3)

    # Insert an extra vertex at (2, 1) that is easier to move around without
    # having to change the entire surface.
    square = square.insertAt(flatsurf.HalfEdge(1), vector(2, 1))

    Edge = flatsurf.Edge
    shift = lambda e: vector(-64, -64) if e in [Edge(4), Edge(5), Edge(6)] else vector(0, 0)

    capsys.readouterr()

    with pytest.raises(cppyy.gbl.std.invalid_argument):
        square + [shift(e) for e in square.edges()]

    square + [shift(e) / 128 for e in square.edges()]

    # Verify that #179 has been fixed, i.e., no warnings are printed
    output = capsys.readouterr()
    assert output.out == ""
    assert output.err == ""

if __name__ == '__main__': sys.exit(pytest.main(sys.argv))
