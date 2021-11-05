######################################################################
# This file is part of flatsurf.
#
#       Copyright (C) 2019-2021 Julian Rüth
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
# methods as they would be destroyed by the garbage collector otherwise
# (segfault.) Eventually we want renf_class/Module to use shared_ptr here. Then
# this won't be a problem anymore.
K = eantic.renf_class.make("x^2 - 3", "x", "1.73 +/- 0.1")
O = eantic.renf_class.make("x^2 - x - 8", "x", "3.37 +/- 0.1")

def square(R2):
    vectors = [R2(1, 0), R2(0, 1), R2(1, 1)]
    vertices = [[1, 3, 2, -1, -3, -2]]
    return Surface(vertices, vectors)

def L(R2):
    vectors = [R2(1, 0), R2(1, 1), R2(0, 1), R2(-1, 0), R2(-1, -1), R2(1, 0), R2(1, 1), R2(0, 1), R2(0, -1)]
    vertices = [[1, 2, 3, 4, 5, -3, 6, 7, 8, -6, -2, 9, -4, -5, -9, -1, -7, -8]]
    return Surface(vertices, vectors)

def hexagon(R2, R=None):
    x = K.gen()
    if R is None:
        R = R2.Coordinate
    vectors = [R2(R(K, 2), R(K, 0)), R2(R(K, 1), x), R2(R(K, 3), x), R2(R(K, 1), -x), R2(R(K, 4), R(K, 0)), R2(R(K, 3), x)]
    vertices = [[1, 3, -4, -5, -3, -2], [2, -1, -6, 4, 5, 6]]
    return Surface(vertices, vectors)

def random_hexagon(R2):
    x = K.gen()
    M = NumberFieldModule(K, RealNumber.rational(1), RealNumber.random(), RealNumber.random())
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

def D33(R2, R=None):
    if R is None:
        R = R2.Coordinate
    zero = R(O, 0)
    one = R(O, 1)
    two = R(O, 2)
    one_fourth = R(O, 1) / R(O, 4)
    three_fourth = R(O, 3) / R(O, 4)
    eleven_fourth = R(O, 11) / R(O, 4)
    fifteen_fourth = R(O, 15) / R(O, 4)
    x = O.gen()
    vectors = [R2(x + one_fourth, x), R2(-x, zero), R2(-one_fourth, -x),
               R2(x + three_fourth, 2), R2(-x, zero), R2(-three_fourth, -two),
               R2(one, two), R2(-one_fourth, zero), R2(-three_fourth, -two),
               R2(one, two), R2(x - fifteen_fourth, zero), R2(eleven_fourth - x, -two)]
    vertices = [[1, -3, 5, -4, 9, -8, 10, -12, 11, -10, 6, -5],
                [3, -2, 4, -6, 8, -7, 12, -11, 7, -9, 2, -1]]
    return Surface(vertices, vectors)

def surfaces(T=None):
    if T is None: 
        for T in ['long long', 'mpz_class', 'mpq_class', 'eantic::renf_elem_class', 'exactreal::Element<exactreal::IntegerRing>', 'exactreal::Element<exactreal::RationalField>', 'exactreal::Element<exactreal::NumberField>']:
            for surface in surfaces(T):
                yield surface
    else:
        R2 = flatsurf.Vector[T]

        import cppyy

        surface = square(R2)
        surface.name = f"Square[{T}]"
        yield surface

        surface = L(R2)
        surface.name = f"L[{T}]"
        yield surface

        if R2 == flatsurf.Vector['long long']:
            pass
        elif R2 == flatsurf.Vector[cppyy.gbl.mpz_class]:
            pass
        elif R2 == flatsurf.Vector[cppyy.gbl.mpq_class]:
            pass
        elif R2 == flatsurf.Vector[cppyy.gbl.exactreal.Element[cppyy.gbl.exactreal.IntegerRing]]:
            pass
        elif R2 == flatsurf.Vector[cppyy.gbl.exactreal.Element[cppyy.gbl.exactreal.RationalField]]:
            pass
        elif R2 == flatsurf.Vector[cppyy.gbl.eantic.renf_elem_class]:
            surface = hexagon(R2, R=eantic.renf_elem)
            surface.name = f"Hexagon[{T}]"
            yield surface

            surface = D33(R2, R=eantic.renf_elem)
            surface.name = f"D33[{T}]"
            yield surface
            pass
        elif R2 == flatsurf.Vector[cppyy.gbl.exactreal.Element[cppyy.gbl.exactreal.NumberField]]:
            surface = random_hexagon(R2)
            surface.name = f"Hexagon[{T}]"
            yield surface
        else:
            raise TypeError(R2)

def name(surface):
    return surface.name
