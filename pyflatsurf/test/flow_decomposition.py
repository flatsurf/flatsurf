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
    decompositions = {(1, 0): 0, (2, 0): 0}

    for connection in surface.connections().bound(16).sector(flatsurf.HalfEdge(1)):
        decomposition = flatsurf.makeFlowDecomposition(surface, connection.vector())
        assert repr(decomposition).startswith("FlowDecomposition")
        assert str(decomposition).startswith("FlowDecomposition")
        decomposition.decompose(-1)
        n_cylinders = 0
        n_without_periodic_trajectory = 0
        for component in decomposition.components():
            n_cylinders += bool(component.cylinder() == True)
            n_without_periodic_trajectory += bool(component.withoutPeriodicTrajectory() == True)

            if component.cylinder():
                # 3 ways to compute area
                area1 = component.area()

                h = component.circumferenceHolonomy()
                p0 = [p for p in component.perimeter()][0]
                sc = p0.saddleConnection()
                p0 = sc.vector()
                vertical = component.vertical()
                assert vertical.projectPerpendicular(h) == 0
                assert vertical.project(h) > 0
                area2 = p0.x() * h.y() - p0.y() * h.x()

                v = vertical.vertical()
                vx = v.x()
                vy = v.y()
                area3 = vertical.projectPerpendicular(p0) * vertical.project(h)

                assert area1 == 2 * area2
                assert area3 == area2 * (vx*vx + vy*vy)

        assert all(component.cylinder() for component in decomposition.cylinders())
        assert len(decomposition.cylinders()) == n_cylinders

        assert all(component.withoutPeriodicTrajectory() for component in decomposition.minimalComponents())
        assert len(decomposition.minimalComponents()) == n_without_periodic_trajectory

        assert len(decomposition.undeterminedComponents()) == 0

        t = (n_cylinders, n_without_periodic_trajectory)
        decompositions[t] += 1

        if n_without_periodic_trajectory == 0:
            assert decomposition.parabolic()

    assert decompositions == {(1, 0): 7, (2, 0): 3}

def test_D33():
    S = surfaces.D33()
    decompositions = {(2, 0): 0, (3, 0): 0, (0, 2): 0}
    for connection in S.connections().bound(4):
        v = connection.vector()
        decomposition = flatsurf.makeFlowDecomposition(S, v)
        assert repr(decomposition).startswith("FlowDecomposition")
        assert str(decomposition).startswith("FlowDecomposition")
        decomposition.decompose(-1)
        n_cylinders, n_without_periodic_trajectory, n_indeterminate = 0, 0, 0
        for dec in decomposition.components():
            if dec.cylinder() == True:
                n_cylinders += 1
                h = dec.circumferenceHolonomy()
                assert h.x() or h.y()
                assert h.x() * v.y() == h.y() * v.x()
            elif dec.cylinder() == False:
                n_without_periodic_trajectory += 1
            else:
                n_indeterminate = 0
        assert n_indeterminate == 0
        decompositions[(n_cylinders, n_without_periodic_trajectory)] += 1

        if not n_without_periodic_trajectory:
            if n_cylinders == 2:
                assert decomposition.parabolic() == True
            elif n_cylinders == 3:
                assert decomposition.parabolic() == False

        assert all(component.cylinder() for component in decomposition.cylinders())
        assert len(decomposition.cylinders()) == n_cylinders

        assert all(component.withoutPeriodicTrajectory() for component in decomposition.minimalComponents())
        assert len(decomposition.minimalComponents()) == n_without_periodic_trajectory

        assert len(decomposition.undeterminedComponents()) == 0

    assert decompositions == {(0, 2): 8, (2, 0): 20, (3, 0): 12}


def test_undetermined():
    S = surfaces.D33()

    R2 = flatsurf.Vector['eantic::renf_elem_class']
    decomposition = flatsurf.makeFlowDecomposition(S, R2(135, 17))
    assert len(decomposition.undeterminedComponents()) == 1
    decomposition.decompose(lambda component: True, -1)
    assert len(decomposition.undeterminedComponents()) == 1
    decomposition.components()[0].decompose(lambda component: True, -1)
    assert len(decomposition.undeterminedComponents()) == 1
    decomposition.components()[0].decompose(-1)
    assert len(decomposition.undeterminedComponents()) == 0


if __name__ == '__main__': sys.exit(pytest.main(sys.argv))
