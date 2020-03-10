# -*- coding: utf-8 -*-
#*********************************************************************
#  This file is part of flatsurf.
#
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

def stratum(surface):
    r"""
    Return the ambient stratum.

    EXAMPLES::

        sage: from pyflatsurf import Surface
        sage: import flatsurf as sage_flatsurf
        sage: T = sage_flatsurf.polygons.triangle(1, 2, 4)
        sage: S = sage_flatsurf.similarity_surfaces.billiard(T)
        sage: S = S.minimal_cover(cover_type="translation")
        sage: S = Surface(S)
        sage: S.stratum()
        H_3(3, 1, 0)
    """
    sings = [surface.angle(v)-1 for v in surface.vertices()]
    from surface_dynamics import AbelianStratum
    return AbelianStratum(*sings)

