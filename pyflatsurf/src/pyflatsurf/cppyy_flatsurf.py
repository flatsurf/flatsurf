# -*- coding: utf-8 -*-
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

import cppyy
from cppyy.gbl import std

from pyexactreal import exactreal
from pyexactreal.cppyy_exactreal import enable_arithmetic, pretty_print, add_pythonization

from .pythonization import enable_iterable, enable_vector_print, add_saddle_connections, enable_hash

from cppyythonizations.pickling.cereal import enable_cereal

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
add_pythonization(enable_arithmetic, "flatsurf", lambda proxy, name: name.startswith("Vector<"))
cppyy.py.add_pythonization(pretty_print, "flatsurf")
cppyy.py.add_pythonization(enable_hash, "flatsurf")
cppyy.py.add_pythonization(enable_vector_print, "flatsurf")
cppyy.py.add_pythonization(add_saddle_connections, "flatsurf")
cppyy.py.add_pythonization(lambda proxy, name: enable_cereal(proxy, name, ["flatsurf/cereal.hpp"]), "flatsurf")

def add_cylinder_diagram(proxy, name):
    if proxy.__cpp_name__.startswith("flatsurf::FlowDecomposition<flatsurf::FlatTriangulation<"):
        from .decomposition import cylinder_diagram
        proxy.cylinder_diagram = cylinder_diagram

cppyy.py.add_pythonization(add_cylinder_diagram, "flatsurf")

def add_stratum(proxy, name):
    if proxy.__cpp_name__.startswith("flatsurf::FlatTriangulation<"):
        from .surface import stratum
        proxy.stratum = stratum

def add_deformation(proxy, name):
    if proxy.__cpp_name__.startswith("flatsurf::FlatTriangulation<"):
        from .surface import deformation
        proxy.__add__ = deformation

cppyy.py.add_pythonization(add_stratum, "flatsurf")
cppyy.py.add_pythonization(add_deformation, "flatsurf")

for path in os.environ.get('PYFLATSURF_INCLUDE','').split(':'):
    if path: cppyy.add_include_path(path)

cppyy.include("flatsurf/cppyy.hpp")

from cppyy.gbl import flatsurf
