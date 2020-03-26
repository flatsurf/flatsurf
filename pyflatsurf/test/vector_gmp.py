#!/usr/bin/env python3
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

import sys
import pytest
import ctypes

from pyflatsurf import flatsurf
from pyeantic import eantic

def test_creation():
    v = flatsurf.Vector['mpz_class'](1, 1)
    assert str(v) == "(1, 1)"

    import cppyy
    mpq = cppyy.gbl.mpq_class
    mpz = cppyy.gbl.mpz_class
    v = flatsurf.Vector[mpq](1, (mpq(mpz(1), mpz(3))))
    assert str(v) == "(1, 1/3)"

if __name__ == '__main__': sys.exit(pytest.main(sys.argv))

