#!/usr/bin/env python3
# -*- coding: utf-8 -*-

######################################################################
# This file is part of flatsurf.
#
#       Copyright (C) 2020 Julian Rüth
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

from pyflatsurf import flatsurf
import surfaces

def test_hexagon_eantic():
    surface = surfaces.hexagon()
    decompositions = {}

    for connection in surface.saddle_connections(flatsurf.Bound(16, 0), flatsurf.HalfEdge(1)):
        decomposition = flatsurf.makeFlowDecomposition(surface, connection.vector())
        flatsurf.decomposeFlowDecomposition(decomposition, -1)
        n_cylinders = 0
        n_without_periodic_components = 0
        for component in decomposition.components():
            n_cylinders += bool(component.cylinder() == True)
            n_without_periodic_components += bool(component.withoutPeriodicTrajectory() == True)

            if component.cylinder():
                # 3 ways to compute area
                area1 = component.area()

                h = component.circumferenceHolonomy()
                p0 = [p for p in component.perimeter()][0]
                sc = p0.saddleConnection()
                p0 = sc.vector()
                vertical = component.vertical()
                assert vertical.perpendicular(h) == 0
                assert vertical.parallel(h) > 0
                area2 = p0.x() * h.y() - p0.y() * h.x()

                v = vertical.vertical()
                vx = v.x()
                vy = v.y()
                area3 = vertical.perpendicular(p0) * vertical.parallel(h)

                assert area1 == 2 * area2
                assert area3 == area2 * (vx*vx + vy*vy)

        t = (n_cylinders, n_without_periodic_components)
        if t not in decompositions:
            decompositions[t] = 0
        decompositions[t] += 1

    assert decompositions == {(1, 0): 7, (2, 0): 3}

if __name__ == '__main__': sys.exit(pytest.main(sys.argv))
