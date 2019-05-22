# -*- coding: utf-8 -*-
######################################################################
#  This file is part of flatsurf.
#
#        Copyright (C) 2019 Julian Rüth
#
#  flatsurf is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 of the License, or
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

from pyflatsurf import flatsurf
from cppyy.gbl import std

def square(R2):
    vectors = [R2(1, 0), R2(0, 1), R2(1, 1)]
    vertices = [[1, 3, 2, -1, -3, -2]]
    return flatsurf.Surface(vertices, vectors)
