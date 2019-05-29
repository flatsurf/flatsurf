# -*- coding: utf-8 -*-
#*********************************************************************
#  This file is part of flatsurf.
#
#        Copyright (C) 2019 Julian Rüth
#
#  Flatsurf is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 of the License, or
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

import cppyy
from cppyy.gbl import std

from pyexactreal import exactreal

# Importing cysignals after cppyy gives us proper stack traces on segfaults
# whereas cppyy otherwise only reports "segmentation violation" (which is
# probably what cling provides.)
import os
if os.environ.get('PYFLATSURFL_CYSIGNALS', True):
    try:
        import cysignals
    except ModuleNotFoundError:
        pass

def make_iterable(proxy, name):
    if hasattr(proxy, 'begin') and hasattr(proxy, 'end'):
        if not hasattr(proxy, '__iter__'):
            def iter(self):
                i = self.begin()
                while i != self.end():
                    if hasattr(i, '__deref__'):
                        yield i.__deref__()
                    elif hasattr(i, 'dereference'):
                        yield i.dereference()
                    else:
                        raise Exception("iterator has no deref method")
                    if hasattr(i, '__preinc__'):
                        i.__preinc__()
                    elif hasattr(i, 'increment'):
                        i.increment()
                    else:
                        raise Exception("iterator has not preinc method")

            proxy.__iter__ = iter

        if not hasattr(proxy, '__len__'):
            def len(self):
                return std.distance(self.begin(), self.end())

            proxy.__len__ = len

cppyy.py.add_pythonization(make_iterable, "flatsurf")

def enable_arithmetic(proxy, name):
    if name.startswith('Vector'):
        for (n, op) in [('add', ord('+')), ('sub', ord('-')), ('mul', ord('*')), ('div', ord('/'))]:
            def cppname(x):
                # some types such as int do not have a __cppname__; there might
                # be a better way to get their cppname but this seems to work
                # fine for the types we're using at least.
                return type(x).__cppname__ if hasattr(type(x), '__cppname__') else type(x).__name__
            def binary(lhs, rhs, op = op):
                return cppyy.gbl.exactreal.boost_binary[cppname(lhs), cppname(rhs), op](lhs, rhs)
            def inplace(lhs, *args, **kwargs): raise NotImplementedError("inplace operators are not supported yet")
            setattr(proxy, "__%s__"%n, binary)
            setattr(proxy, "__r%s__"%n, binary)
            setattr(proxy, "__i%s__"%n, inplace)
        setattr(proxy, "__neg__", lambda self: cppyy.gbl.exactreal.minus(self))

cppyy.py.add_pythonization(enable_arithmetic, "flatsurf")

def pretty_print(proxy, name):
    proxy.__repr__ = proxy.__str__

cppyy.py.add_pythonization(pretty_print, "flatsurf")

for path in os.environ.get('PYFLATSURF_INCLUDE','').split(':'):
    if path: cppyy.add_include_path(path)

cppyy.include("flatsurf/cppyy.hpp")

from cppyy.gbl import flatsurf

def make_surface(vertices, vectors):
    vertices = std.vector[std.vector[int]]([std.vector[int](l) for l in vertices])
    R2 = type(vectors[0]).__cppname__
    vectors = std.vector[R2](vectors)

    # Somehow vectors[0].Coordinate cannot be found, so we have to try which
    # coordinate type is the correct one; we should report this at cppyy if we
    # can find a more easily reproducible case.
    for coordinate in ['long long', 'eantic::renf_elem_class', 'exactreal::Element<exactreal::IntegerRingTraits>', 'exactreal::Element<exactreal::RationalFieldTraits>', 'exactreal::Element<exactreal::NumberFieldTraits>']:
        if R2 == type(flatsurf.Vector[coordinate]).__cppname__:
            ret = flatsurf.FlatTriangulation[coordinate](flatsurf.FlatTriangulationCombinatorial(vertices), vectors)
            ret.saddleConnections = lambda *args: flatsurf.SaddleConnections[type(ret).__cppname__](ret, *args)
            return ret

    raise Exception("Coordinate not implemented in Python wrapper")

flatsurf.Surface = make_surface
