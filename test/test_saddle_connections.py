#*********************************************************************
#  This file is part of Polygon.
#
#        Copyright (C) 2019 Julian Rüth
#
#  Polygon is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 of the License, or
#  (at your option) any later version.
#
#  Polygon is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Polygon. If not, see <https://www.gnu.org/licenses/>.
#*********************************************************************

import pytest

from pypolygon import polygon
from cppyy.gbl import std

@pytest.mark.parametrize("surface,R2Along,radius", [
    ("square",  polygon.VectorAlongTriangulation[polygon.VectorLongLong],  -1),
    ("square",  polygon.VectorAlongTriangulation[polygon.VectorLongLong],   1),
    ("square",  polygon.VectorAlongTriangulation[polygon.VectorLongLong], 100),
    ("square",  polygon.VectorAlongTriangulationWithApproximation[polygon.VectorEAntic, polygon.VectorArb],   100),
    ("square",  polygon.VectorAlongTriangulation[polygon.VectorEAntic],                    100),
    ("square",  polygon.VectorAlongTriangulation[polygon.VectorLongLong], 300),
    ("square",  polygon.VectorAlongTriangulationWithApproximation[polygon.VectorEAntic, polygon.VectorArb],   300),
    ("hexagon", polygon.VectorAlongTriangulationWithApproximation[polygon.VectorEAntic, polygon.VectorArb],   100),
    ("hexagon", polygon.VectorAlongTriangulation[polygon.VectorEAntic],                    100),
    ("hexagon", polygon.VectorAlongTriangulationWithApproximation[polygon.VectorEAntic, polygon.VectorArb],   300),
])
def test_search_saddle_connections(surface, R2Along, radius, benchmark):
    if R2Along == polygon.VectorAlongTriangulation[polygon.VectorLongLong]:
        R2 = polygon.VectorLongLong
    elif R2Along == polygon.VectorAlongTriangulationWithApproximation[polygon.VectorEAntic, polygon.VectorArb]:
        R2 = polygon.VectorEAntic
    elif R2Along == polygon.VectorAlongTriangulation[polygon.VectorEAntic]:
        R2 = polygon.VectorEAntic

    if R2 == polygon.VectorLongLong:
        one = 1
    elif R2 == polygon.VectorEAntic:
        from cppyy.gbl import renf_class, renf_elem_class
        one = renf_elem_class(1)
        if surface == "hexagon":
            K = renf_class("x^2 - 3", "x", "1.73 +/- 0.1")
            x = K.gen()
    else:
        raise NotImplementedError(repr(R2))

    VI = std.vector[int]
    VVI = std.vector[VI]
    VR2 = std.vector[R2]
    if surface == "square":
        vectors = VR2([ R2(one, one*0), R2(one*0, one), R2(one, one) ])
        vertices = VVI([VI([1, 3, 2, -1, -3, -2])])
    elif surface == "hexagon":
        vectors = VR2([ R2(one*2, one*0), R2(one, x), R2(one*3, x), R2(one, -x), R2(one*4, one*0), R2(one*3, x) ])
        vertices = VVI([VI([1, 3, -4, -5, -3, -2]), VI([2, -1, -6, 4, 5, 6])])
    else:
        raise NotImplementedError(surface)
        
    triangulation = polygon.FlatTriangulation[R2](vertices, vectors)

    if surface == "square":
        from math import gcd
        expected = len([(x,y) for x in range(2, radius) for y in range(0, x) if x**2 + y**2 < radius**2 and gcd(x, y) == 1])
    elif surface == "hexagon":
        expected = len(list(polygon.SaddleConnections[R2, polygon.VectorAlongTriangulation[R2]](triangulation, polygon.Bound(radius*radius), polygon.HalfEdge(1))))

    # I think I prefer asv over pytest.benchmark
    # However, pytest.benchmark has the better interface (from a programming perspective) whereas asv has the nicer and more useful web UI
    # import yep
    # import tempfile
    # yep.start(tempfile.mkstemp(suffix='yep', prefix="%s-%s-%s"%(surface, R2, radius))[1])
    n = benchmark(lambda: len(list(polygon.SaddleConnections[R2, R2Along](triangulation, polygon.Bound(radius*radius), polygon.HalfEdge(1)))))
    # yep.stop()
    assert n == expected
