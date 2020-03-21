# -*- coding: utf-8 -*-
######################################################################
#  This file is part of flatsurf.
#
#        Copyright (C) 2020 Vincent Delecroix
#
#  flatsurf is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  flatsurf is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with flatsurf. If not, see <https://www.gnu.org/licenses/>.
######################################################################

def cylinder_diagram(decomposition):
    r"""
    Return the combinatorics of the associated cylinder diagram (from ``surface_dynamics``).
    """
    if decomposition.completelyPeriodic() != True:
        raise ValueError("not a completely periodic direction")

    indices = {}
    saddle_connections = []
    cyls = []
    k = 0
    for c in decomposition.components():
        contour = [p for p in c.perimeter()]
        ibot = 0
        assert not contour[ibot].vertical()
        itop = 1
        while contour[itop].vertical():
            itop += 1
        assert all(contour[i].vertical() for i in range(itop+1, len(contour)))
        bot = []
        for i in range(ibot+1, itop):
            c = contour[i]
            if c not in indices:
                indices[c] = k
                indices[-c] = k
                saddle_connections.append(c.saddleConnection().vector())
                k += 1
            bot.append(indices[contour[i]])
        top = []
        for i in range(itop+1, len(contour)):
            c = contour[i]
            if c not in indices:
                indices[c] = k
                indices[-c] = k
                saddle_connections.append((-c.saddleConnection()).vector())
                k += 1
            top.append(indices[contour[i]])
        cyls.append((bot, top))

    from surface_dynamics import CylinderDiagram
    return CylinderDiagram(cyls), saddle_connections
