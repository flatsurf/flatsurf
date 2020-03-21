# -*- coding: utf-8 -*-
#*********************************************************************
#  This file is part of flatsurf.
#
#        Copyright (C) 2020 Vincent Delecroix
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

def stratum(surface):
    r"""
    Return the ambient stratum.

    EXAMPLES::

        sage: from pyflatsurf import Surface
        sage: import flatsurf as sage_flatsurf
        sage: T = sage_flatsurf.polygons.triangle(1, 2, 4)
        sage: S = sage_flatsurf.similarity_surfaces.billiard(T)
        sage: S = S.minimal_cover(cover_type="translation")
        sage: S = Surface(S)
        sage: S.stratum()
        H_3(3, 1, 0)
    """
    sings = [surface.angle(v)-1 for v in surface.vertices()]
    from surface_dynamics import AbelianStratum
    return AbelianStratum(*sings)

# TODO: move at C++ level with v either a std::vector<Vector<T>>
def deformation(surface, v):
    r"""
    A surface in the deformation given by ``v``.

    EXAMPLES::

        sage: from pyflatsurf import flatsurf, Surface
        sage: from gmpxxyy import mpq
        sage: import cppyy
        sage: verts = [[int(1),int(-3),int(2),int(-1),int(3),int(-2)]]
        sage: v1 = flatsurf.Vector[mpq](1,0)
        sage: v2 = flatsurf.Vector[mpq](0,1)
        sage: v3 = flatsurf.Vector[mpq](-1,-1)
        sage: surface = Surface(verts, [v1, v2, v3])
        sage: surface
        FlatTriangulationCombinatorial(vertices = (1, -3, 2, -1, 3, -2), faces = (1, 2, 3)(-1, -2, -3)) with vectors 1: (1, 0), 2: (0, 1), 3: (-1, -1)
        sage: d1 = flatsurf.Vector[mpq]('1/4', 0)
        sage: d2 = flatsurf.Vector[mpq]('-1/4', '1/4')
        sage: d3 = flatsurf.Vector[mpq](0, '-1/4')
        sage: surface + [d1,d2,d3]
        FlatTriangulationCombinatorial(vertices = (1, -3, 2, -1, 3, -2), faces = (1, 2, 3)(-1, -2, -3)) with vectors 1: (5/4, 0), 2: (-1/4, 5/4), 3: (-1, -5/4)
    """
    n = surface.size()
    if len(v) != n:
        raise ValueError("the number of vectors should match the number of edges of the surface")

    from .vector import Vectors
    import cppyy
 
    def atVertex(S, v):
         outgoing = [S.outgoing(v)[0]]
         while S.nextAtVertex(outgoing[-1]) != outgoing[0]:
             outgoing.append(S.nextAtVertex(outgoing[-1]))
         return outgoing

    vertices = [[he.id() for he in atVertex(surface, v)] for v in surface.vertices()]
    holonomies = [surface.fromEdge(e.positive()) for e in surface.edges()]

    for i in range(n):
        holonomies[i] = holonomies[i] + v[i]

    return cppyy.gbl.flatsurf.makeFlatTriangulation(vertices, holonomies)
