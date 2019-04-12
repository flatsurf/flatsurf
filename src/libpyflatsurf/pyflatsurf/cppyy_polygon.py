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

# Importing cysignals after cppyy gives us proper stack traces on segfaults
# whereas cppyy otherwise only reports "segmentation violation" (which is
# probably what cling provides.)
import os
if os.environ.get('PYFLATSURF_CYSIGNALS', True):
    import cysignals

def make_iterable(proxy, name):
    if hasattr(proxy, 'begin') and hasattr(proxy, 'end'):
        if not hasattr(proxy, '__iter__'):
            def iter(self):
                i = self.begin()
                while i != self.end():
                    yield i.__deref__()
                    i.__preinc__()

            proxy.__iter__ = iter

cppyy.py.add_pythonization(make_iterable, "flatsurf")

if 'PYFLATSURF_INCLUDE' in os.environ:
    cppyy.add_include_path(os.environ['PYFLATSURF_INCLUDE'])

cppyy.include("libflatsurf/cppyy.hpp")
cppyy.include("e-antic/renfxx.h")

from cppyy.gbl import flatsurf
