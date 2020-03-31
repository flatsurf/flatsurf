# -*- coding: utf-8 -*-
#*********************************************************************
#  This file is part of flatsurf.
#
#        Copyright (C) 2019-2020 Julian Rüth
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

import re

import cppyy
from cppyy.gbl import std

from pyexactreal import exactreal
from pyexactreal.cppyy_exactreal import pretty_print

from .pythonization import enable_iterable, enable_vector_print, enable_hash

from cppyythonizations.pickling.cereal import enable_cereal
from cppyythonizations.util import filtered, add_method
from cppyythonizations.operators.arithmetic import enable_arithmetic

# Importing cysignals after cppyy gives us proper stack traces on segfaults
# whereas cppyy otherwise only reports "segmentation violation" (which is
# probably what cling provides.)
import os
if os.environ.get('PYFLATSURF_CYSIGNALS', True):
    try:
        import cysignals
    except ModuleNotFoundError:
        pass

cppyy.py.add_pythonization(enable_iterable, "flatsurf")
cppyy.py.add_pythonization(filtered(re.compile("Vector<.*>"))(enable_arithmetic), "flatsurf")
cppyy.py.add_pythonization(pretty_print, "flatsurf")
cppyy.py.add_pythonization(enable_hash, "flatsurf")
cppyy.py.add_pythonization(enable_vector_print, "flatsurf")
cppyy.py.add_pythonization(lambda proxy, name: enable_cereal(proxy, name, ["flatsurf/cereal.hpp"]), "flatsurf")

cppyy.py.add_pythonization(filtered(re.compile("FlatTriangulation<.*>"))(add_method("saddle_connections")(lambda self, *args: cppyy.gbl.flatsurf.SaddleConnections[type(self)](self, *args))), "flatsurf")

cppyy.py.add_pythonization(filtered(re.compile("FlatTriangulation<.*>"))(add_method("insertAt")(lambda self, *args: cppyy.gbl.flatsurf.insertAtFlatTriangulation(self, *args))), "flatsurf")
cppyy.py.add_pythonization(filtered(re.compile("FlatTriangulation<.*>"))(add_method("slot")(lambda self, *args: cppyy.gbl.flatsurf.slotFlatTriangulation(self, *args))), "flatsurf")

cppyy.py.add_pythonization(filtered(re.compile("FlowDecomposition<.*>"))(add_method("decompose")(lambda self, *args: cppyy.gbl.flatsurf.decomposeFlowDecomposition(self, *args))), "flatsurf")

for path in os.environ.get('PYFLATSURF_INCLUDE','').split(':'):
    if path: cppyy.add_include_path(path)

cppyy.include("flatsurf/cppyy.hpp")

from cppyy.gbl import flatsurf
