#!/usr/bin/env python3
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

from pyflatsurf import flatsurf, Surface
import flatsurf as sage_flatsurf
from pyeantic.sage_conversion import sage_nf_to_eantic, sage_nf_elem_to_eantic

def test_unfold_triangle():
    # Unfold the (5, 6, 7) triangle with sage-flatsurf
    T = sage_flatsurf.polygons.triangle(5, 6, 7)
    S = sage_flatsurf.similarity_surfaces.billiard(T)
    S = S.minimal_cover(cover_type="translation")

    # TODO: Make this a method in sage_flatsurf_conversion.py

    # Map sage-flatsurf's (face, id) to flatsurf.HalfEdge 
    edges = {}
    # Map flatsurf.HalfEdge to flatsurf.Vector
    vectors = {}

    K = None

    for face in S.label_iterator():
        for edge in [0, 1, 2]:
            label = (face, edge)

            if label in edges: continue

            edges[label] = len(edges) // 2 + 1
            edges[S.opposite_edge(*label)] = -edges[label]

            vector = S.polygon(face).edge(edge)

            if K is None:
                K = sage_nf_to_eantic(vector[0].parent())

            vector = flatsurf.Vector['eantic::renf_elem_class'](
                sage_nf_elem_to_eantic(K, vector[0]),
                sage_nf_elem_to_eantic(K, vector[1]))

            vectors[edges[label]] = vector
            vectors[edges[S.opposite_edge(*label)]] = -vector

    # The vectors for the HalfEdge 1 … n
    vectors = [vectors[i + 1] for i in range(len(vectors) // 2)]

    # Construct half edge permutation around vertices
    vertices = {}

    for face in S.label_iterator():
        for edge in [0, 1, 2]:
            label = (face, edge)
            nextLabel = S.opposite_edge(face, (edge + 2) % 3)
            vertices[edges[label]] = edges[nextLabel]

    cycles = []
    while vertices:
        key = next(iter(vertices.keys()))
        cycle = []
        while True:
            cycle.append(key)
            successor = vertices[key]
            del vertices[key]
            key = successor 
            if key == cycle[0]: break
        cycles.append(cycle)

    vertices = cycles

    surface = Surface(vertices, vectors)
    assert(str(surface) == "")

    return surface

if __name__ == '__main__': sys.exit(pytest.main(sys.argv))
