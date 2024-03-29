#!/usr/bin/env python

######################################################################
# This file is part of flatsurf.
#
#       Copyright (C) 2020 Julian Rüth
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

def test_edge():
    half_edge = flatsurf.HalfEdge(1)
    assert half_edge.edge().positive() == half_edge

if __name__ == '__main__': sys.exit(pytest.main(sys.argv))
