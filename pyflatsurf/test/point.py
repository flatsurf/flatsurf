#!/usr/bin/env python3

######################################################################
# This file is part of flatsurf.
#
#       Copyright (C) 2022 Julian Rüth
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

import cppyy
import sys
import pytest

from pyflatsurf import flatsurf
import surfaces


def test_point_constructor():
    for coefficients in ['long long', 'mpz_class', 'mpq_class', 'eantic::renf_elem_class', 'exactreal::Element<exactreal::IntegerRing>', 'exactreal::Element<exactreal::RationalField>', 'exactreal::Element<exactreal::NumberField>']:
        T = getattr(cppyy.gbl, coefficients)

        surface = surfaces.square(flatsurf.Vector[coefficients])
        Surface = type(surface)

        p = flatsurf.Point[Surface](surface, flatsurf.HalfEdge(1), T(1), T(0), T(0))
        q = flatsurf.Point[Surface](surface, flatsurf.HalfEdge(1), T(0), T(1), T(0))
        assert p == q

def test_point_coordinates():
    for coefficients in ['long long', 'mpz_class', 'mpq_class', 'eantic::renf_elem_class', 'exactreal::Element<exactreal::IntegerRing>', 'exactreal::Element<exactreal::RationalField>', 'exactreal::Element<exactreal::NumberField>']:
        T = getattr(cppyy.gbl, coefficients)

        surface = surfaces.square(flatsurf.Vector[coefficients])
        Surface = type(surface)

        p = flatsurf.Point[Surface](surface, flatsurf.HalfEdge(1), T(1), T(0), T(0))

        face = p.face()
        coordinates = p.coordinates(face)
        assert coordinates[0] == 1
        assert coordinates[1] == 0
        assert coordinates[2] == 0
        assert tuple(coordinates) == (1, 0, 0)

def test_point_flow():
    for coefficients in ['long long', 'mpz_class', 'mpq_class', 'eantic::renf_elem_class', 'exactreal::Element<exactreal::IntegerRing>', 'exactreal::Element<exactreal::RationalField>', 'exactreal::Element<exactreal::NumberField>']:
        T = getattr(cppyy.gbl, coefficients)

        surface = surfaces.square(flatsurf.Vector[coefficients]).applyMatrix(T(2), T(0), T(0), T(2)).codomain()
        Surface = type(surface)

        p = flatsurf.Point[Surface](surface, flatsurf.HalfEdge(1), T(1), T(1), T(0))
        shift = flatsurf.Vector[T](T(1), T(0))
        q = p + shift

        assert p != q

        q -= shift

        assert p == q

        q += 2 * shift

        assert p == q


if __name__ == '__main__': sys.exit(pytest.main(sys.argv))
