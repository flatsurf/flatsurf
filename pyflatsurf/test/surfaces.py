# -*- coding: utf-8 -*-
######################################################################
# This file is part of flatsurf.
#
#       Copyright (C) 2019 Julian Rüth
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

from pyflatsurf import flatsurf, Surface
from pyexactreal import RealNumber, NumberFieldModule
import pyexactreal
from pyexactreal import exactreal
from pyeantic import eantic

# The number field has to live out here and not be a local variable in the
# methods as it would be destroyed by the garbage collector otherwise
# (segfault.) Eventually we want renf_class/Module to use shared_ptr here. Then
# this won't be a problem anymore.
K = eantic.renf_class.make("x^2 - 3", "x", "1.73 +/- 0.1")

def square(R2):
    vectors = [R2(1, 0), R2(0, 1), R2(1, 1)]
    vertices = [[1, 3, 2, -1, -3, -2]]
    return Surface(vertices, vectors)

def L(R2):
    vectors = [R2(1, 0), R2(1, 1), R2(0, 1), R2(-1, 0), R2(-1, -1), R2(1, 0), R2(1, 1), R2(0, 1), R2(0, -1)]
    vertices = [[1, 2, 3, 4, 5, -3, 6, 7, 8, -6, -2, 9, -4, -5, -9, -1, -7, -8]]
    return Surface(vertices, vectors)

def hexagon():
    R2 = flatsurf.Vector['eantic::renf_elem_class']
    x = K.gen()
    R = eantic.renf_elem
    vectors = [R2(R(K, 2), R(K, 0)), R2(R(K, 1), x), R2(R(K, 3), x), R2(R(K, 1), -x), R2(R(K, 4), R(K, 0)), R2(R(K, 3), x)]
    vertices = [[1, 3, -4, -5, -3, -2], [2, -1, -6, 4, 5, 6]]
    return Surface(vertices, vectors)

def random_hexagon():
    x = K.gen()
    M = NumberFieldModule(K, RealNumber.rational(1), RealNumber.random(), RealNumber.random())
    R2 = flatsurf.Vector['exactreal::Element<exactreal::NumberField>']
    # The side lengths are going to be 2, 2·μ, 2·ν where μ,ν are the random parameters of M.
    one = M.gen(0)
    μ = M.gen(1)
    ν = M.gen(2)
    # We build our vectors from (2, 0), μ·(1, √3), ν.(-1, √3).
    u = R2(2*one, 0*one)
    v = R2(μ, x*μ)
    w = R2(-ν, x*ν)
    vectors = [u, v, u + v, -w, u + v - w, u+v]
    vertices = [[1, 3, -4, -5, -3, -2], [2, -1, -6, 4, 5, 6]]
    return Surface(vertices, vectors)
