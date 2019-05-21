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

    return flatsurf.FlatTriangulation[R2](flatsurf.FlatTriangulationCombinatorial(vertices), vectors)

flatsurf.Surface = make_surface