# -*- coding: utf-8 -*-
r"""
Vector wrapper and sage conversion for ``flatsurf.Vector``.
"""
#*********************************************************************
#  This file is part of flatsurf.
#
#        Copyright (C) 2020-2021 Vincent Delecroix
#                      2020 Julian Rüth
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
import gmpxxyy

from pyflatsurf import flatsurf
from gmpxxyy import mpz, mpq
from pyeantic import eantic, real_embedded_number_field
from pyexactreal.exact_reals import ExactReals

from sage.all import ZZ, QQ, FreeModule, FreeModules, Morphism, Hom, SetsWithPartialMaps, NumberFields, Parent, UniqueRepresentation
from sage.rings.number_field.number_field_base import NumberField as SageNumberField
from sage.structure.element import Vector as SageVector
from sage.structure.richcmp import op_NE, op_EQ

class Vector(SageVector):
    r"""
    A two-dimensional vector that wraps ``flatsurf.Vector``.

    EXAMPLES::

        sage: from pyflatsurf import flatsurf
        sage: from pyflatsurf.vector import Vectors

    A vector with integer coordinates::

        sage: from gmpxxyy import mpz
        sage: V = Vectors(ZZ)
        sage: V(1, 2)
        (1, 2)
        sage: V(flatsurf.Vector[mpz](1, 2))
        (1, 2)

    A vector with rational coordinates::

        sage: from gmpxxyy import mpz, mpq
        sage: V = Vectors(QQ)
        sage: V(1/3, -2/7)
        (1/3, -2/7)
        sage: V(flatsurf.Vector[mpq]('1/3', '-2/7'))
        (1/3, -2/7)

    A vector with number field coordinates::

        sage: from pyeantic import eantic, RealEmbeddedNumberField
        sage: x = polygen(QQ)
        sage: K = NumberField(x^3 - 2, 'a', embedding=AA(2)**(1/3))
        sage: R = RealEmbeddedNumberField(K)
        sage: V = Vectors(K)
        sage: x = R(K.gen())
        sage: y = R(K.gen()**2 - 2/3)
        sage: V(x, y)
        ((a ~ 1.2599210), (a^2 - 2/3 ~ 0.92073439))
        sage: V(flatsurf.Vector[eantic.renf_elem_class](x.renf_elem, y.renf_elem))
        ((a ~ 1.2599210), (a^2 - 2/3 ~ 0.92073439))

    A vector with real coordinates::

        sage: from pyexactreal import ExactReals
        sage: R = ExactReals()
        sage: V = Vectors(R)
        sage: x = R.random_element()
        sage: y = R.random_element()
        sage: V(x, y)
        (ℝ(0...…), ℝ(0...…))

    """
    def __init__(self, parent, vector, y = None):
        SageVector.__init__(self, parent)

        R = parent.base_ring()

        if y is not None:
            vector = (vector, y)

        if isinstance(vector, parent.Vector):
            self.vector = vector
        elif isinstance(vector, flatsurf.cppyy.Vector[parent.Vector.Coordinate]):
            self.vector = vector
        else:
            vector = list(vector)

            if len(vector) != 2:
                raise ValueError("vector must have exactly two coordinate entries")

            elif isinstance(vector[0], parent.coordinate) and isinstance(vector[1], parent.coordinate):
                self.vector = parent.Vector(*vector)
            else:
                self.vector = parent.Vector(parent._to_coordinate(vector[0]), parent._to_coordinate(vector[1]))

    def _repr_(self):
        return repr(self.vector)

    def monomial_coefficients(self, *args, **kwargs):
        r"""
        Return the coefficients of this vector.

        EXAMPLES::

            sage: from pyflatsurf.vector import Vectors
            sage: V = Vectors(ZZ)
            sage: v = V(13, 37)
            sage: v.monomial_coefficients()
            {0: 13, 1: 37}

        """
        coefficients = {}
        x = self.parent().base_ring()(self.vector.x())
        y = self.parent().base_ring()(self.vector.y())
        if x:
            coefficients[0] = x
        if y:
            coefficients[1] = y
        return coefficients

    def __iter__(self):
        r"""
        EXAMPLES::

            sage: from pyflatsurf.vector import Vectors
            sage: V = Vectors(ZZ)
            sage: v = V(13, 37)
            sage: list(v)
            [13, 37]

        """
        yield self.parent().base_ring()(self.vector.x())
        yield self.parent().base_ring()(self.vector.y())

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
        scalar = self.parent()._to_coordinate(scalar)

        return self.parent()(scalar * self.vector.x(), scalar * self.vector.y())

    _lmul_ = _rmul_

    def _acted_upon_(self, c, self_on_left):
        r"""
        Act upon this vector with ``c``.

        This method is used to implement multiplication with scalars that
        cannot be handled by the coercion framework directly.

        EXAMPLES:

        The real embedded number field at the base of this vector space coerces
        into the corresponding SageMath number field. However, the converse
        coercion does not exist so multiplication of such SageMath number field
        elements with vectors here would not work otherwise::

            sage: from pyflatsurf.vector import Vectors
            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x**2 - 2, 'a', embedding=sqrt(AA(2)))
            sage: R = RealEmbeddedNumberField(K)
            sage: R.has_coerce_map_from(K)
            False
            sage: V = Vectors(K)
            sage: v = V.an_element()
            sage: K.gen() * v in V
            True

        """
        isomorphic_base_ring = self.parent()._isomorphic_vector_space.base_ring()
        if isomorphic_base_ring is not self.base_ring():
            if c in isomorphic_base_ring:
                return self.base_ring()(c) * self

        return None

    def __bool__(self):
        r"""
        Return whether this is a non-zero vector.

        EXAMPLES::

            sage: from pyflatsurf.vector import Vectors
            sage: V = Vectors(ZZ)
            sage: bool(V(0, 0))
            False
            sage: bool(V(1, 0))
            True

        """
        return bool(self.vector)

    def _richcmp_(self, other, op):
        r"""
        TESTS::

            sage: from pyflatsurf import flatsurf
            sage: from pyflatsurf.vector import Vectors
            sage: from gmpxxyy import mpz
            sage: V = Vectors(ZZ)
            sage: V(1, 2) == V(1, 2)
            True
            sage: V(1, 2) == V(1, 1)
            False
            sage: V(1, 1) < V(1, 2)
            Traceback (most recent call last):
            ...
            TypeError: unsupported comparison between vectors
        """
        if op == op_EQ:
            return self.vector == other.vector
        elif op == op_NE:
            return self.vector != other.vector
        else:
            raise TypeError('unsupported comparison between vectors')


class Vectors(UniqueRepresentation, Parent):
    r"""
    The subspace of the vector space `\mathbb{R}^2` modeled by vectors with
    entries in ``base_ring``. Unlike normal SageMath vector spaces, this space
    is backed by flatsur's ``Vector`` class with entries in ``coordinate``.

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
        sage: Vectors(R) is V
        True
        sage: Vectors(R.renf) is V
        True

        sage: from gmpxxyy import mpz, mpq
        sage: Vectors(QQ) is Vectors(mpq)
        True
        sage: Vectors(ZZ) is Vectors(mpz)
        True
    """
    Element = Vector

    @staticmethod
    def __classcall__(cls, base_ring):
        if base_ring in [ZZ, mpz]:
            base_ring = ZZ
            coordinate = mpz
        elif base_ring in [QQ, mpq]:
            base_ring = QQ
            coordinate = mpq
        elif isinstance(base_ring, real_embedded_number_field.RealEmbeddedNumberField):
            coordinate = cppyy.gbl.eantic.renf_elem_class
        elif isinstance(base_ring, eantic.renf_class):
            base_ring = real_embedded_number_field.RealEmbeddedNumberField(base_ring)
            coordinate = cppyy.gbl.eantic.renf_elem_class
        elif isinstance(base_ring, SageNumberField):
            base_ring = real_embedded_number_field.RealEmbeddedNumberField(base_ring)
            coordinate = cppyy.gbl.eantic.renf_elem_class
        elif isinstance(base_ring, ExactReals):
            coordinate = base_ring._element_factory
        else:
            raise NotImplementedError("unsupported base_ring {!r}".format(base_ring))

        return super(Vectors, cls).__classcall__(cls, base_ring, coordinate)

    def __init__(self, base_ring, coordinate):
        self.coordinate = coordinate

        self.Vector = flatsurf.Vector._unwrapped[self.coordinate]

        self._isomorphic_vector_space = FreeModule(base_ring, 2)
        if isinstance(base_ring, real_embedded_number_field.RealEmbeddedNumberField):
            self._isomorphic_vector_space = FreeModule(base_ring.number_field, 2)

        Parent.__init__(self, base_ring, category=FreeModules(base_ring))

        self.register_coercion(self._isomorphic_vector_space)
        self._isomorphic_vector_space.register_conversion(ConversionVectorSpace(self))

    def _algebraic_ring(self):
        r"""
        Return the Sage algebraic ring which underlies our base ring; typically
        this is (an isomorphic ring to) the base ring itself but for
        non-algebraic rings such as ``ExactReals``, this gives the coefficient
        ring.

        EXAMPLES::

            sage: from pyflatsurf.vector import Vectors
            sage: from pyexactreal import ExactReals

            sage: Vectors(QQ)._algebraic_ring()
            Rational Field
            sage: R = ExactReals()
            sage: Vectors(R)._algebraic_ring()
            Rational Field

            sage: K = NumberField(x^3 - 2, 'a', embedding=AA(2)**(1/3))
            sage: Vectors(K)._algebraic_ring()
            Number Field in a with defining polynomial x^3 - 2 with a = 1.259921049894873?
            sage: R = ExactReals(K)
            sage: Vectors(R)._algebraic_ring()
            Number Field in a with defining polynomial x^3 - 2 with a = 1.259921049894873?
        """
        algebraic_ring = self.base_ring()

        if isinstance(algebraic_ring, ExactReals):
            algebraic_ring = algebraic_ring.base_ring()
        if algebraic_ring not in [ZZ, QQ]:
            algebraic_ring = algebraic_ring.number_field

        return algebraic_ring

    def decomposition(self, vector, base=None):
        r"""
        Write ``vector`` as a shortest sum `v = \sum a_i v_i` where the
        `a_i` are coefficients from our ``base_ring()`` and the `v_i` are
        vectors with entries in ``base``.

        EXAMPLES::

            sage: from pyflatsurf.vector import Vectors
            sage: V = Vectors(ZZ)
            sage: V.decomposition([13, 37])
            [(1, (13, 37))]
            sage: V.decomposition([0, 0])
            []

        ::

            sage: from pyexactreal import ExactReals
            sage: R = ExactReals()
            sage: V = Vectors(R)
            sage: V.decomposition([R.random_element(), R.random_element()])
            [(ℝ(0.178808…), (1, 0)), (ℝ(0.478968…), (0, 1))]

        ::

            sage: from pyeantic import RealEmbeddedNumberField
            sage: K = NumberField(x^3 - 2, 'a', embedding=AA(2)**(1/3))
            sage: V = Vectors(K)
            sage: V.decomposition([0, 1])
            [(1, (0, 1))]

        ::

            sage: R = ExactReals(K)
            sage: V = Vectors(R)
            sage: V.decomposition([K.random_element() * R.random_element(), R.random_element(), R.random_element()])
            [(ℝ(0.621222…), (0, 1, 0)),
             (ℝ(0.673083…), (0, 0, 1)),
             (ℝ(0.782515…), (..., 0, 0))]

        """
        vector = tuple(self._to_coordinate(x) for x in vector)

        if not any(vector): return []

        if base is None:
            base = self._algebraic_ring()

        if base is self.base_ring():
            return [(self.base_ring().one(), tuple(map(base, vector)))]

        if hasattr(self.base_ring(), "number_field") and self.base_ring().number_field is base:
            return [(self.base_ring().one(), tuple(base(self.base_ring()(x)) for x in vector))]

        if isinstance(self.base_ring(), ExactReals):
           from functools import reduce
           span = type(vector[0].module()).span
           module = reduce(lambda x,y: span(x, y.module()), vector, vector[0].module())

           vector = [entry.promote(module).coefficients() for entry in vector]
           vector = [[base(Vectors(base).base_ring()(c)) for c in coefficients] for coefficients in vector]

           V = base**len(vector)
           return [(self.base_ring()(module.gen(i)), tuple(V(coefficients))) for (i, coefficients) in enumerate(zip(*vector)) if any(coefficients)]

        raise NotImplementedError("cannot decompose vector in %s over %s"%(self, base))

    def _to_coordinate(self, x):
        r"""
        Convert ``x`` to something that the flatsurf backend for this vector type understands.

        EXAMPLES::

            sage: from pyflatsurf.vector import Vectors
            sage: V = Vectors(QQ)
            sage: type(V._to_coordinate(1))
            <class cppyy.gbl.__gmp_expr<__mpz_struct[1],__mpz_struct[1]> at ...>
            sage: type(V._to_coordinate(1/2))
            <class cppyy.gbl.__gmp_expr<__mpq_struct[1],__mpq_struct[1]> at ...>

        """
        if isinstance(x, self.coordinate):
            return x
        if isinstance(x, cppyy.gbl.mpz_class):
            return x
        if isinstance(x, cppyy.gbl.mpq_class):
            return x
        if isinstance(self.base_ring(), real_embedded_number_field.RealEmbeddedNumberField):
            # We create a copy of the coordinate. Otherwise, cppyy (as of
            # 1.9.5) treats this as an r-value and moves x out even if it is
            # referenced elsewhere.
            value = self.base_ring()(x).renf_elem
            return type(value)(value)
        if isinstance(self.base_ring(), ExactReals):
            return self.base_ring()(x)._backend
        if x in ZZ:
            return cppyy.gbl.mpz_class(str(x))
        if x in QQ:
            return cppyy.gbl.mpq_class(str(x))

        raise NotImplementedError("Cannot convert %s to something the flatsurf backend understands yet, i.e., cannot convert a %s into a %s"%(x, type(x), type(self.coordinate)))

    def _repr_(self):
        r"""
        Return a printable representation of this vector space.

        EXAMPLES::

            sage: from pyflatsurf.vector import Vectors
            sage: V = Vectors(QQ)
            sage: V
            Flatsurf Vectors over Rational Field

        """
        return "Flatsurf Vectors over %r"%(self.base_ring(),)

    def an_element(self):
        return self((1,0))

    def zero(self):
        r"""
        Return the zero vector.

        EXAMPLES::

            sage: from pyflatsurf.vector import Vectors
            sage: V = Vectors(QQ)
            sage: V.zero()
            (0, 0)
        """
        return self((0,0))


class ConversionVectorSpace(Morphism):
    r"""
    A conversion from flatsurf ``Vectors`` to SageMath ``VectorSpace``.
    """
    def __init__(self, domain):
        Morphism.__init__(self, Hom(domain, domain._isomorphic_vector_space, SetsWithPartialMaps()))

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
        return self.codomain()((R(x), R(y)))
