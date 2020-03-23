#!/usr/bin/env python3
# -*- coding: utf-8 -*-

######################################################################
#  This file is part of flatsurf.
#
#        Copyright (C) 2019-2020 Julian Rüth
#                      2020 Vincent Delecroix
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

# TODO: Turn parts of this into a benchmark

import argparse
import sys

import cppyy
from sage.all import gcd, QQ, Permutations
from sage.arith.all import gcd
from pyflatsurf import flatsurf, Surface, GL2ROrbitClosure
import flatsurf as sage_flatsurf
from pyeantic.sage_conversion import sage_nf_to_eantic, sage_nf_elem_to_eantic

parser = argparse.ArgumentParser(description='Survey a Triangle')
parser.add_argument('angles', metavar='N', type=int, nargs='+')
parser.add_argument('--bound', type=int, default=10)

args = parser.parse_args()

assert len(args.angles) == 4
a,b,c,d = args.angles
if gcd([a, b, c, d]) != 1:
    raise ValueError

def sample_lengths(E):
    L = E.lengths_polytope()
    r1, r2 = [v.vector() for v in L.rays()]
    for i in range(4):
        if r1[i] == 0:
            continue
        for j in range(4):
            if r2[j] == 0:
                continue
            yield r1 / r1[i] + r2 / r2[j]

for q in Permutations(args.angles):
    if q[0] != d:
        continue

    print("angles %s"%(q))
    E = sage_flatsurf.EquiangularPolygons(*q)
    ambient_locus = E.billiard_unfolding_stratum("half-translation", True)
    stratum = E.billiard_unfolding_stratum("translation", True)
    quad = ambient_locus != stratum
    ambient_dim = ambient_locus.dimension()

    for lengths in sample_lengths(E):
        P = E(lengths)
        S = sage_flatsurf.similarity_surfaces.billiard(P)
        S = S.minimal_cover(cover_type="translation")
        S, _ = S.normalized_coordinates()
        assert S.stratum() == stratum, (a, b, c, d, S.stratum(), stratum)

        # Dynamical orbit closure computation
        O = GL2ROrbitClosure(S)
        print(O.surface) # underlying pyflatsurf surface

        sc_completely_periodic = True
        cyl_completely_periodic = True
        parabolic = True
        undetermined = 0
        explored = 0

        # computation
        dim = O.U.dimension()
        for d in O.decompositions_depth_first(args.bound, 1024):
            print("Investigating in direction %s "%d.u, end='')
            sys.stdout.flush()
            explored += 1

            ncyl, nmin, nund = d.num_cylinders_minimals_undetermined()
            print("decomposes into %d cylinders and %d minimal components and %d undetermined" % (ncyl, nmin, nund))
            if nund:
                undetermined += 1
                continue

            if sc_completely_periodic or cyl_completely_periodic or parabolic:
                cyl_completely_periodic = cyl_completely_periodic and (ncyl == 0 or nmin == 0)
                sc_completely_periodic = sc_completely_periodic and (nmin == 0)

                if ncyl and parabolic:
                    parabolic = parabolic and d.parabolic()

            O.update_tangent_space_from_flow_decomposition(d)
            new_dim = O.U.dimension()
            if new_dim != dim:
                dim = new_dim
                print("new tangent vector: dim=%d/%d"%(dim, ambient_dim))
            if dim == ambient_locus.dimension() and not cyl_completely_periodic:
                print("nothing more to be investigated")
                break
        else:
            print("up to bound %s on the length of saddle connection"%args.bound)

        closure_dim = O.U.dimension()
        absolute_dim = O.absolute_dimension()
        print("Unfolding %s with lengths %s"%(args.angles, lengths))
        print("%d directions explored (%d undetermined)"%(explored, undetermined))
        if quad:
            print("ambient locus: %s inside %s (of dimension %s)"%(ambient_locus, ambient_locus.orientation_cover(), ambient_dim))
        else:
            print("ambient locus: %s (of dimension %s)"%(ambient_locus, ambient_dim))
        print("orbit closure dimension: %d"%closure_dim)
        print("rank: %s"%QQ((absolute_dim,2)))
        if sc_completely_periodic and undetermined:
            print("saddle connection completely periodic: ? (probably false)")
        else:
            print("saddle connection completely periodic: %s"%sc_completely_periodic)
        if cyl_completely_periodic and undetermined:
            print("cylinder completely periodic: ? (probably true)")
        else:
            print("cylinder completely periodic: %s"%cyl_completely_periodic)
        if parabolic and undetermined:
            print("parabolic: ? (probably true)")
        else:
            print("parabolic: %s"%parabolic)
