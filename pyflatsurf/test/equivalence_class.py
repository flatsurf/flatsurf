#!/usr/bin/env python

######################################################################
# This file is part of flatsurf.
#
#       Copyright (C) 2022-2023 Julian Rüth
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

import sys
import pytest

from pyflatsurf import flatsurf
import surfaces


def test_equivalence_class():
    for coefficients in ['long long', 'mpz_class', 'mpq_class', 'eantic::renf_elem_class', 'exactreal::Element<exactreal::IntegerRing>', 'exactreal::Element<exactreal::RationalField>', 'exactreal::Element<exactreal::NumberField>']:
        square = surfaces.square(flatsurf.Vector[coefficients])
        l = surfaces.L(flatsurf.Vector[coefficients])
        S = type(square)
        for equivalence in [flatsurf.Equivalence[S].combinatorial(), flatsurf.Equivalence[S].linear()]:
            a = flatsurf.EquivalenceClass[S](square, equivalence)
            b = flatsurf.EquivalenceClass[S](l, equivalence)

            assert a != b
            assert hash(a) != hash(b)
            assert a == flatsurf.EquivalenceClass[S](square, equivalence)
            assert hash(a) == hash(flatsurf.EquivalenceClass[S](square, equivalence))

if __name__ == '__main__': sys.exit(pytest.main(sys.argv))
