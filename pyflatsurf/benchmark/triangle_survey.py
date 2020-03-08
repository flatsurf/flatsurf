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
from sage.all import gcd, QQ
from sage.arith.all import gcd
from pyflatsurf import flatsurf, Surface, GL2ROrbitClosure
import flatsurf as sage_flatsurf
from pyeantic.sage_conversion import sage_nf_to_eantic, sage_nf_elem_to_eantic
import surface_dynamics

parser = argparse.ArgumentParser(description='Survey a Triangle')
parser.add_argument('angles', metavar='N', type=int, nargs='+')
parser.add_argument('--bound', type=int, default=10)

args = parser.parse_args()

assert len(args.angles) == 3
a,b,c = args.angles
if gcd([a, b, c]) != 1:
    raise ValueError

print("Unfolding %s"%(args.angles))

# Unfold the (a, b, c) triangle with sage-flatsurf
T = sage_flatsurf.polygons.triangle(*args.angles)
S = sage_flatsurf.similarity_surfaces.billiard(T)
S = S.minimal_cover(cover_type="translation")

# Dynamical orbit closure computation
O = GL2ROrbitClosure(S)
print(O.surface) # underlying pyflatsurf surface

# prediction for the orbit closure
# (this is an upper bound)
N = n = a + b + c
a = QQ((a,n))
b = QQ((b,n))
c = QQ((c,n))
if N%2:
    N *= 2

quad = False # whether we have a quadratic differential
angles = {} # angle in the cover (angle, mult)
for x in [a,b,c]:
    d = x.denominator()
    y = x.numerator()
    if d%2:
        y *= 2
        d *= 2
    quad = quad or (y % 2 == 1)
    mult = N // d
    if y in angles:
        angles[y] += N//d
    else:
        angles[y] = N//d

if quad:
    quad = True
    ambient_locus = surface_dynamics.QuadraticStratum({y-2: mult for y,mult in angles.items()})
else:
    quad = False
    ambient_locus = surface_dynamics.AbelianStratum({(y-2)//2: mult for y,mult in angles.items()})

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
        print("new tangent vector: dim=%d"%dim)
    if dim == ambient_locus.dimension() and not cyl_completely_periodic:
        print("nothing more to be investigated")
        break
else:
    print("up to bound %s on the length of saddle connection"%args.bound)

closure_dim = O.U.dimension()
absolute_dim = O.absolute_dimension()
print("Unfolding %s"%(args.angles))
print("%d directions explored (%d undetermined)"%(explored, undetermined))
if quad:
    print("ambient locus: %s inside %s (of dimension %s)"%(ambient_locus, ambient_locus.orientation_cover(), ambient_locus.dimension()))
else:
    print("ambient locus: %s (of dimension %s)"%(ambient_locus, ambient_locus.dimension()))
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
