# -*- coding: utf-8 -*-
#*********************************************************************
#  This file is part of flatsurf.
#
#        Copyright (C) 2019 Julian Rüth
#
#  Flatsurf is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Flatsurf is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with flatsurf. If not, see <https://www.gnu.org/licenses/>.
#*********************************************************************

from cppyy.gbl import std
from .cppyy_flatsurf import flatsurf

def make_FlatTriangulation(vertices, vectors):
    vertices = std.vector[std.vector[int]]([std.vector[int](l) for l in vertices])
    R2 = type(vectors[0]).__cppname__
    vectors = std.vector[R2](vectors)

    # Somehow vectors[0].Coordinate cannot be found, so we have to try which
    # coordinate type is the correct one; we should report this at cppyy if we
    # can find a more easily reproducible case.
    for coordinate in ['long long', 'eantic::renf_elem_class', 'exactreal::Element<exactreal::IntegerRing>', 'exactreal::Element<exactreal::RationalField>', 'exactreal::Element<exactreal::NumberField>']:
        if R2 == type(flatsurf.Vector[coordinate]).__cppname__:
            ret = std.make_shared[flatsurf.FlatTriangulation[coordinate]](std.move(flatsurf.FlatTriangulationCombinatorial(vertices)), vectors)
            ret.saddleConnections = lambda *args: flatsurf.SaddleConnections[type(ret).__cppname__](ret, *args)
            return ret

    raise Exception("Coordinate not implemented in Python wrapper")

def make_surface(surface_or_vertices, vectors = None):
    from collections import Iterable
    if isinstance(surface_or_vertices, Iterable) and isinstance(vectors, Iterable):
        return make_FlatTriangulation(surface_or_vertices, vectors)
    elif surface_or_vertices.__module__ == "flatsurf.geometry.translation_surface":
        if vectors is not None:
            raise ValueError("vectors must be none when creating a FlatTriangulation from a SageMath flatsurf surface")
        from .sage_conversion import to_FlatTriangulation
        return to_FlatTriangulation(surface_or_vertices)
    else:
        raise TypeError("Unsupported parameters to make_surface()")
