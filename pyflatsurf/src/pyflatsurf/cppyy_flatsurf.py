# -*- coding: utf-8 -*-
#*********************************************************************
#  This file is part of flatsurf.
#
#        Copyright (C) 2019-2020 Julian Rüth
#        Copyright (C) 2020 Vincent Delecroix
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

from .pythonization import enable_iterable

from cppyythonizations.pickling.cereal import enable_cereal
from cppyythonizations.util import filtered, add_method, wrap_method
from cppyythonizations.operators.arithmetic import enable_arithmetic
from cppyythonizations.printing import enable_pretty_printing, enable_list_printing

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
cppyy.py.add_pythonization(filtered(re.compile("Vector<.*>"))(add_method("__str__")(lambda self: "(" + str(self.x()) + ", " + str(self.y()) + ")")), "flatsurf")
cppyy.py.add_pythonization(enable_pretty_printing, "flatsurf")
cppyy.py.add_pythonization(lambda proxy, name: enable_cereal(proxy, name, ["flatsurf/cereal.hpp"]), "flatsurf")
cppyy.py.add_pythonization(filtered(re.compile("vector<flatsurf::.*>"))(enable_list_printing), "std")


# We cannot create an OddHalfEdgeMap directly due to https://bitbucket.org/wlav/cppyy/issues/273/segfault-in-cpycppyy-anonymous-namespace
cppyy.py.add_pythonization(filtered(re.compile("FlatTriangulation<.*>"))(wrap_method("__add__")(lambda self, cpp, rhs: cpp(cppyy.gbl.flatsurf.makeOddHalfEdgeMap[cppyy.gbl.flatsurf.Vector[type(self).Coordinate]](self.combinatorial(), rhs)))), "flatsurf")


# We cannot call decompose() directly due to https://bitbucket.org/wlav/cppyy/issues/273/segfault-in-cpycppyy-anonymous-namespace
cppyy.py.add_pythonization(filtered(re.compile("FlowDecomposition<.*>"))(add_method("decompose")(lambda self, *args: cppyy.gbl.flatsurf.decomposeFlowDecomposition(self, *args))), "flatsurf")
cppyy.py.add_pythonization(filtered(re.compile("FlowDecomposition<.*>"))(add_method("cylinders")(lambda self: [component for component in self.components() if component.cylinder()])), "flatsurf")
cppyy.py.add_pythonization(filtered(re.compile("FlowDecomposition<.*>"))(add_method("minimalComponents")(lambda self: [component for component in self.components() if component.withoutPeriodicTrajectory()])), "flatsurf")
cppyy.py.add_pythonization(filtered(re.compile("FlowDecomposition<.*>"))(add_method("undeterminedComponents")(lambda self: [component for component in self.components() if not (component.cylinder() == True) and not (component.withoutPeriodicTrajectory() == True)])), "flatsurf")
cppyy.py.add_pythonization(filtered(re.compile("FlowDecomposition<.*>"))(add_method("__str__")(lambda self: "FlowDecomposition with %d cylinders, %d minimal components and %d undetermined components" % (len(self.cylinders()), len(self.minimalComponents()), len(self.undeterminedComponents())))), "flatsurf")


for path in os.environ.get('PYFLATSURF_INCLUDE','').split(':'):
    if path: cppyy.add_include_path(path)


cppyy.include("flatsurf/cppyy.hpp")


from cppyy.gbl import flatsurf
