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

def enable_iterable(proxy, name):
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
                return cppyy.gbl.std.distance(self.begin(), self.end())

            proxy.__len__ = len

def enable_pretty_print(proxy, name):
    proxy.__repr__ = proxy.__str__

# see https://bitbucket.org/wlav/cppyy/issues/170/std-hash-is-not-picked-up-by-__hash__
def enable_hash(proxy, name):
    proxy.__hash__ = lambda self: cppyy.gbl.std.hash[proxy]()(self)

# Work around https://bitbucket.org/wlav/cppyy/issues/112/operator-for-a-base-class-is-not-found
def enable_vector_print(proxy, name):
    if name.startswith("Vector<"):
        proxy.__str__ = lambda self: "(" + str(self.x()) + ", " + str(self.y()) + ")"
        proxy.__repr__ = proxy.__str__

def add_saddle_connections(proxy, name):
    if name.startswith("FlatTriangulation<"):
        def saddle_connections(self, *args):
            return cppyy.gbl.flatsurf.SaddleConnections[type(self)](self, *args)
        proxy.saddle_connections = saddle_connections

def share_unique_ptr(proxy, name):
    if name.startswith("FlatTriangulation<"):
        # cppyy gets the lifetime of the surfaces wrong when methods return a unique_ptr<Surface>
        # See https://github.com/flatsurf/flatsurf/issues/148 for the upstream issue.
        def insertAt(self, *args):
            return cppyy.gbl.flatsurf.insertAt(self, *args)
        proxy.insertAt = insertAt

        def slot(self, *args):
            return cppyy.gbl.flatsurf.slot(self, *args)
        proxy.slot = slot
