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

import cppyy

from pyflatsurf import flatsurf
import pyeantic
import pyeantic.real_embedded_number_field

from sage.all import ZZ, QQ, FreeModule, FreeModules, Morphism, Hom, SetsWithPartialMaps
from sage.rings.number_field.number_field_base import NumberField as SageNumberField
from sage.structure.element import Vector as SageVector
from sage.structure.parent import Parent
from sage.structure.unique_representation import UniqueRepresentation

class Vector(SageVector):
    r"""
    A two-dimensional vector that wraps ``flatsurf.Vector``.
    """
    def __init__(self, parent, value):
        SageVector.__init__(self, parent)
        R = parent.base_ring()
        if isinstance(value, parent.Vector):
            self.vector = value
        elif R is ZZ or R is QQ:
            self.vector = parent.Vector(parent.datatype(str(value[0])), parent.datatype(str(value[1])))
        elif isinstance(R, pyeantic.real_embedded_number_field.RealEmbeddedNumberField):
            self.vector = parent.Vector(R(value[0]).renf_elem, R(value[1]).renf_elem)
        else:
            raise NotImplementedError("unsupported cofficient type")

    def _repr_(self):
        return repr(self.vector)

    def _add_(self, other):
        r"""
        TESTS::

            sage: from pyflatsurf.vector import Vectors
            sage: V = Vectors(QQ)
            sage: V((1,1)) + V((2,-1))
            (3, 0)
        """
        P = self.parent()
        return P.element_class(P, self.vector + other.vector)

    def _sub_(self, other):
        r"""
        TESTS::

            sage: from pyflatsurf.vector import Vectors
            sage: V = Vectors(QQ)
            sage: V((1,1)) - V((2,-1))
            (-1, 2)
        """
        P = self.parent()
        return P.element_class(P, self.vector - other.vector)

    def _neg_(self):
        r"""
        TESTS::

            sage: from pyflatsurf.vector import Vectors
            sage: V = Vectors(QQ)
            sage: -V((1,-2))
            (-1, 2)
        """
        P = self.parent()
        return P.element_class(P, -self.vector)

    def _rmul_(self, scalar):
        r"""
        TESTS::

            sage: from pyflatsurf.vector import Vectors

            sage: V = Vectors(ZZ)
            sage: 3 * V((1,2))
            (3, 6)

            sage: V = Vectors(QQ)
            sage: V((2, 5)) / 7
            (2/7, 5/7)

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: R = RealEmbeddedNumberField(K)
            sage: V = Vectors(K)
            sage: v = V((1,K.gen() - 3/2))
            sage: v
            (1, (a-3/2 ~ -0.085786438))
            sage: 3 * v
            (3, (3*a-9/2 ~ -0.25735931))
            sage: K.gen() * v
            ((a ~ 1.4142136), (-3/2*a+2 ~ -0.12132034))
            sage: R.gen() * v
            ((a ~ 1.4142136), (-3/2*a+2 ~ -0.12132034))

            sage: v * 3
            (3, (3*a-9/2 ~ -0.25735931))
            sage: v * K.gen()
            ((a ~ 1.4142136), (-3/2*a+2 ~ -0.12132034))
            sage: v * R.gen()
            ((a ~ 1.4142136), (-3/2*a+2 ~ -0.12132034))

            sage: v / 3
            ((1/3 ~ 0.33333333), (1/3*a-1/2 ~ -0.028595479))
            sage: v / K.gen()
            ((1/2*a ~ 0.70710678), (-3/4*a+1 ~ -0.060660172))
            sage: v / R.gen()
            ((1/2*a ~ 0.70710678), (-3/4*a+1 ~ -0.060660172))
        """
        P = self.parent()
        R = P.base_ring()
        x = self.vector.x()
        y = self.vector.y()
        if R is ZZ or R is QQ:
            return P.element_class(P, (P.datatype(str(scalar)) * x, P.datatype(str(scalar)) * y))
        else:
            return P.element_class(P, (scalar.renf_elem * x, scalar.renf_elem * y))

    _lmul_ = _rmul_

class Vectors(UniqueRepresentation, Parent):
    r"""
    EXAMPLES::

        sage: from pyflatsurf.vector import Vectors
        sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
        sage: V = Vectors(K)
        sage: v = V((1,K.gen() - 3/2))
        sage: v
        (1, (a-3/2 ~ -0.085786438))
        sage: VectorSpace(K, 2)(v)
        (1, a - 3/2)

        sage: from pyeantic import RealEmbeddedNumberField
        sage: R = RealEmbeddedNumberField(K)
        sage: assert Vectors(R) is V
    """
    Element = Vector

    @staticmethod
    def __classcall__(cls, base_ring, sage_base_ring=None, datatype=None):
        if datatype is None:
            if base_ring is ZZ:
                from gmpxxyy import mpz
                sage_base_ring = ZZ
                datatype = mpz
            elif base_ring is QQ:
                from gmpxxyy import mpq
                sage_base_ring = QQ
                datatype = mpq
            elif isinstance(base_ring, pyeantic.real_embedded_number_field.RealEmbeddedNumberField):
                datatype = cppyy.gbl.eantic.renf_elem_class
                sage_base_ring = base_ring.number_field
            elif isinstance(base_ring, SageNumberField):
                sage_base_ring = base_ring
                base_ring = pyeantic.RealEmbeddedNumberField(base_ring)
                datatype = cppyy.gbl.eantic.renf_elem_class
            else:
                raise ValueError
        return super(Vectors, cls).__classcall__(cls, base_ring, sage_base_ring, datatype)

    def __init__(self, base_ring, sage_base_ring, datatype):
        self.sage_base_ring = sage_base_ring
        self.datatype = datatype
        self.Vector = flatsurf.Vector[self.datatype]
        self.vector_space = FreeModule(sage_base_ring, 2)
        Parent.__init__(self, base_ring, category=FreeModules(base_ring))
        self.register_coercion(self.vector_space)
        self.vector_space.register_conversion(ConversionVectorSpace(self))

    def _repr_(self):
        return repr(self.vector_space)

    def an_element(self):
        return self((1,0))

class ConversionVectorSpace(Morphism):
    r"""
    A conversion from flatsurf ``Vectors`` to SageMath ``VectorSpace``.
    """
    def __init__(self, domain):
        Morphism.__init__(self, Hom(domain, domain.vector_space, SetsWithPartialMaps()))

    def _call_(self, v):
        r"""
        TESTS::

            sage: from pyflatsurf.vector import Vectors

            sage: V = Vectors(ZZ)
            sage: v = V((1,-2))
            sage: FreeModule(ZZ, 2)(v)
            (1, -2)

            sage: V = Vectors(QQ)
            sage: v = V((1, 2/3))
            sage: VectorSpace(QQ, 2)(v)
            (1, 2/3)

            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: V = Vectors(K)
            sage: v = V((1, K.gen() - 3/2))
            sage: VectorSpace(K, 2)(v)
            (1, a - 3/2)
        """
        x = v.vector.x()
        y = v.vector.y()
        R = self.domain().base_ring()
        if R is ZZ or R is QQ:
            x = str(x)
            y = str(y)
        return self.codomain()((R(x), R(y)))
