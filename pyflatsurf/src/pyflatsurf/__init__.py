# -*- coding: utf-8 -*-
r"""
A Python Interface to libflatsurf

EXAMPLES:

The translation surface coming from a square::

    >>> from pyflatsurf import Surface, flatsurf
    >>> R2 = flatsurf.Vector['long long']
    >>> square = Surface([[1, 3, 2, -1, -3, -2]], [R2(1, 0), R2(0, 1), R2(1, 1)])

TESTS:

Pickling works::

    >>> from pickle import loads, dumps

    >>> v = R2(13, 37)
    >>> loads(dumps(v)) == v
    True

    >>> loads(dumps(square)) == square
    True

"""
#*********************************************************************
#  This file is part of flatsurf.
#
#        Copyright (C) 2019 Julian Rüth
#
#  Flatsurf is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Flatsurf is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with flatsurf. If not, see <https://www.gnu.org/licenses/>.
#*********************************************************************

from .cppyy_flatsurf import flatsurf

from .factory import make_surface as Surface
