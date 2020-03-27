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

import cppyy
from .cppyy_flatsurf import flatsurf

def make_FlatTriangulation(vertices, vectors):
    vertices = cppyy.gbl.std.vector[cppyy.gbl.std.vector[int]]([cppyy.gbl.std.vector[int](l) for l in vertices])
    R2 = type(vectors[0])
    vectors = cppyy.gbl.std.vector[R2](vectors)
    return cppyy.gbl.flatsurf.makeFlatTriangulation(vertices, vectors);

def make_surface(surface_or_vertices, vectors = None):
    from collections.abc import Iterable
    if hasattr(surface_or_vertices, "__module__") and surface_or_vertices.__module__ == "flatsurf.geometry.translation_surface":
        if vectors is not None:
            raise ValueError("vectors must be none when creating a FlatTriangulation from a SageMath flatsurf surface")
        from .sage_conversion import to_FlatTriangulation
        return to_FlatTriangulation(surface_or_vertices)
    elif isinstance(surface_or_vertices, Iterable) and isinstance(vectors, Iterable):
        return make_FlatTriangulation(surface_or_vertices, vectors)
    else:
        raise TypeError("Unsupported parameters to make_surface()")
