#!/usr/bin/env python

######################################################################
# This file is part of flatsurf.
#
#       Copyright (C) 2022-2024 Julian Rüth
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
import cppyy

from pyflatsurf import flatsurf
import surfaces


def test_serialization():
    hexagon = surfaces.random_hexagon(flatsurf.Vector['exactreal::Element<exactreal::NumberField>']).combinatorial()

    from pickle import loads, dumps
    clone = loads(dumps(hexagon))
    assert clone == hexagon
    assert hash(clone) == hash(hexagon)


if __name__ == '__main__':
    sys.exit(pytest.main(sys.argv))
