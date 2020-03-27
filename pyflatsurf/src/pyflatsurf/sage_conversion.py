# -*- coding: utf-8 -*-
r"""
Interface with sage-flatsurf

See https://github.com/videlec/sage-flatsurf
"""
#*********************************************************************
#  This file is part of flatsurf.
#
#        Copyright (C) 2019 Vincent Delecroix
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

try:
    from sage.all import QQ
except ImportError:
    raise ImportError("pyflatsurf.sage_conversion needs SageMath, see http://www.sagemath.org/")

try:
    import flatsurf as sage_flatsurf
except ImportError:
    raise ImportError("pyflatsurf.sage_conversion needs sage-flatsurf, see https://github.com/videlec/sage-flatsurf")

from .cppyy_flatsurf import flatsurf
from .factory import make_surface as Surface

def _check_data(vp, fp, vec):
    r"""
    Check consistency of data

    vp - vector permutation
    fp - face permutation
    vec - vectors of the flat structure
    """
    assert isinstance(vp, list)
    assert isinstance(fp, list)
    assert isinstance(vec, list)

    n = len(vp) - 1

    assert n%2 == 0, n
    assert len(fp) == n+1
    assert len(vec) == n//2

    assert vp[0] is None
    assert fp[0] is None

    for i in range(1, n//2+1):
        # check fp/vp consistency
        assert fp[-vp[i]] == i, i

        # check that each face is a triangle and that vectors sum up to zero
        j = fp[i]
        k = fp[j]
        assert i != j and i != k and fp[k] == i, (i, j, k)
        vi = vec[i-1] if i >= 1 else -vec[-i-1]
        vj = vec[j-1] if j >= 1 else -vec[-j-1]
        vk = vec[k-1] if k >= 1 else -vec[-k-1]
        v = vi + vj + vk
        assert v.x() == 0, v.x()
        assert v.y() == 0, v.y()

def _cycle_decomposition(p):
    n = len(p) - 1
    assert n%2 == 0
    cycles = []
    unseen = [True] * (n+1)
    for i in list(range(-n//2+1, 0)) + list(range(1, n//2)):
        if unseen[i]:
            j = i
            cycle = []
            while unseen[j]:
                unseen[j] = False
                cycle.append(j)
                j = p[j]
            cycles.append(cycle)
    return cycles

def make_vectors(vectors, base_ring):
    r"""
    Convert a list of sage vectors to pyflatsurf Vector.
    """
    from .vector import Vectors
    V = Vectors(base_ring)
    return [V(v).vector for v in vectors]

def to_FlatTriangulation(S):
    r"""
    Given S a translation surface from flatsurf builds a flat polygonization

    EXAMPLES::

        sage: import flatsurf
        sage: from pyflatsurf import Surface

    Origamis (defined over QQ)::

        sage: G = SymmetricGroup(2)
        sage: r = u = G('(1,2)')
        sage: O = flatsurf.translation_surfaces.origami(r, u)
        sage: Surface(O)
        FlatTriangulationCombinatorial(vertices = (1, -3, 2, -1, 6, -5)(-2, 4, -6, 5, -4, 3), faces = (1, 2, 3)(-1, -5, -6)(-2, -3, -4)(4, 5, 6)) with vectors 1: (1, 1), 2: (-1, 0), 3: (0, -1), 4: (1, 1), 5: (-1, 0), 6: (0, -1)

        sage: G = SymmetricGroup(3)
        sage: r = G('(1,2,3)')
        sage: u = G('(1,2)')
        sage: O = flatsurf.translation_surfaces.origami(r, u)
        sage: Surface(O)
        FlatTriangulationCombinatorial(vertices = (1, -3, 8, -7, 3, -2, 4, -6, 5, -4, 9, -8, 7, -9, 2, -1, 6, -5), faces = (1, 2, 3)(-1, -5, -6)(-2, -9, -4)(-3, -7, -8)(4, 5, 6)(7, 8, 9)) with vectors 1: (1, 1), 2: (-1, 0), 3: (0, -1), 4: (1, 1), 5: (-1, 0), 6: (0, -1), 7: (1, 1), 8: (-1, 0), 9: (0, -1)

    Building the regular 2n-gons for n=3,5,7 (Veech surfaces)::

        sage: for n in range(3,8,2):
        ....:     S = flatsurf.translation_surfaces.veech_double_n_gon(5)
        ....:     T = Surface(S)

    Arnoux-Yoccoz surfaces in genus 3 and 4::

        sage: A3 = flatsurf.translation_surfaces.arnoux_yoccoz(3)
        sage: B3 = Surface(A3)

    ::

        sage: A4 = flatsurf.translation_surfaces.arnoux_yoccoz(4)
        sage: B4 = Surface(A4)

    Ward surfaces (a regular 2n-gon glued to two regular n-gons)::

        sage: W3 = flatsurf.translation_surfaces.ward(3)
        sage: X3 = Surface(W3)

    ::

        sage: W17 = flatsurf.translation_surfaces.ward(17)
        sage: X17 = Surface(W17)
    """
    if not isinstance(S, sage_flatsurf.geometry.translation_surface.TranslationSurface):
        raise TypeError("S must be a translation surface")
    if not S.is_finite():
        raise ValueError("the surface S must be finite")

    S = S.triangulate()

    # populate half edges and vectors
    n = sum(S.polygon(lab).num_edges() for lab in S.label_iterator())
    half_edge_labels = {}   # map: (face lab, edge num) in faltsurf -> integer
    vec = []                # vectors
    k = 1                   # half edge label in {1, ..., n}
    for t0, t1 in S.edge_iterator(gluings=True):
        if t0 in half_edge_labels:
            continue

        half_edge_labels[t0] = k
        half_edge_labels[t1] = -k

        f0, e0 = t0
        p = S.polygon(f0)
        vec.append(p.edge(e0))

        k += 1

    # compute vertex and face permutations
    vp = [None] * (n+1)  # vertex permutation
    fp = [None] * (n+1)  # face permutation
    for t in S.edge_iterator(gluings=False):
        e = half_edge_labels[t]
        j = (t[1] + 1) % S.polygon(t[0]).num_edges()
        fp[e] = half_edge_labels[(t[0], j)]
        vp[fp[e]] = -e


    # convert the vp permutation into cycles
    verts = _cycle_decomposition(vp)
    vec = make_vectors(vec, S.base_ring())
    _check_data(vp, fp, vec)

    return Surface(verts, vec)
