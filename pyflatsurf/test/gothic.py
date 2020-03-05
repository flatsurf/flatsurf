#!/usr/bin/env python3
# -*- coding: utf-8 -*-
r"""
The gothic locus

From the article McMullen-Mukamel-Wright (2017).
"""

import sys
import pytest

from sage.all import QQ, AA, NumberField, polygen
import flatsurf as sage_flatsurf
from pyflatsurf import GL2ROrbitClosure

def test_gothic_generic():
    x = polygen(QQ)
    K = NumberField(x**3 - 2, 'a', embedding=AA(2)**QQ((1,3)))
    a = K.gen()
    S = sage_flatsurf.translation_surfaces.cathedral(a, a**2)
    O = GL2ROrbitClosure(S)
    for d in O.decompositions(4):
        d.decompose()
        O.update_tangent_space_from_flow_decomposition(d)
    assert O.U.dimension() == O.absolute_dimension() == 4

def test_gothic_veech():
    x = polygen(QQ)
    K = NumberField(x**2 - 2, 'sqrt2', embedding=AA(2)**QQ((1,2)))
    sqrt2 = K.gen()
    x = QQ((1,2))
    y = 1
    a = x + y * sqrt2
    b = -3*x -QQ((3,2)) + 3*y*sqrt2
    S = sage_flatsurf.translation_surfaces.cathedral(a,b)
    O = GL2ROrbitClosure(S)
    for d in O.decompositions(4):
        d.decompose()
        assert d.is_parabolic()
        assert d.cylinder_diagram().stratum() == O.stratum()
        O.update_tangent_space_from_flow_decomposition(d)
    assert O.U.dimension() == 2

if __name__ == '__main__': sys.exit(pytest.main(sys.argv))
