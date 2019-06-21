/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Flatsurf is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with flatsurf. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#include <ostream>

#include "flatsurf/flat_triangulation.hpp"
#include "flatsurf/flat_triangulation_combinatorial.hpp"
#include "flatsurf/half_edge.hpp"
#include "flatsurf/half_edge_map.hpp"
#include "flatsurf/vector.hpp"
#include "flatsurf/vector_along_triangulation.hpp"

#include "vector/algorithm/exact.ipp"
#include "vector/storage/forward.ipp"

using std::ostream;

namespace flatsurf {
namespace {

template <typename Vector, typename Implementation>
class SharedImplementation {
 public:
  using Surface = typename Vector::Surface;

  explicit SharedImplementation(Surface const* surface) : surface(surface) {}

  const Surface* surface;
};

template <typename Vector, typename Implementation, typename Approximation>
class ImplementationWithApproximation : SharedImplementation<Vector, Implementation> {
  static void updateAfterFlip(HalfEdgeMap<int>& map, HalfEdge halfEdge, const FlatTriangulationCombinatorial& parent) {
    map.set(-parent.nextAtVertex(halfEdge), map.get(halfEdge));
    map.set(-parent.nextInFace(parent.nextInFace(halfEdge)), map.get(halfEdge));
    map.set(halfEdge, 0);
  }

 public:
  using Exact = flatsurf::Vector<typename Vector::Coordinate>;
  using Shared = SharedImplementation<Vector, Implementation>;
  using Surface = typename Shared::Surface;

  ImplementationWithApproximation(Surface const* surface) : Shared(surface), coefficients(surface, updateAfterFlip), approx(surface) {}

  Vector operator-() const {
    Vector ret(this->surface);
    ret -= coefficients;
    return ret;
  }

  operator Exact() const {
    using WithoutApproximation = flatsurf::VectorAlongTriangulation<typename Exact::Coordinate>;
    return static_cast<Exact>(WithoutApproximation(this->surface, coefficients));
  }

  auto x() const {
    return static_cast<Exact>(*this).x();
  }

  auto y() const {
    return static_cast<Exact>(*this).y();
  }

  auto approximation() const {
    return static_cast<flatsurf::Vector<Approximation>>(approx);
  }

  auto operator*(const Vector& rhs) const noexcept {
    return static_cast<Exact>(*this) * static_cast<Exact>(rhs);
  }

  auto& operator+=(const HalfEdge e) {
    coefficients.set(e, coefficients.get(e) + 1);
    approx += e;
    return *this;
  }

  auto& operator+=(const HalfEdgeMap<int>& coefficients) {
    approx += coefficients;
    for (auto e : this->surface->halfEdges()) {
      int c = coefficients.get(e);
      if (c > 0) {
        this->coefficients.set(e, this->coefficients.get(e) + c);
      }
    }
    return *this;
  }

 private:
  HalfEdgeMap<int> coefficients;
  flatsurf::VectorAlongTriangulation<Approximation, void, Surface> approx;

  // We could also keep the exact vector around until it changes or buffer the
  // changes in another HalfEdgeMap and only apply these to the exact vector
  // when it's cheaper.
};

template <typename Vector, typename Implementation, typename T>
class ImplementationWithForward : public Forward<flatsurf::Vector<T>>, SharedImplementation<Vector, Implementation> {
 public:
  using Shared = SharedImplementation<Vector, Implementation>;
  using Surface = typename Shared::Surface;
  using Shared::Shared;

  Vector operator-() const {
    Vector ret(this->surface);
    Implementation::impl(ret).vector = -this->vector;
    return ret;
  }

  T operator*(const Vector& rhs) const {
    return this->vector * Implementation::impl(rhs).vector;
  }

  Vector perpendicular() const {
    Vector ret(this->surface);
    Implementation::impl(ret).vector = this->vector.perpendicular();
    return ret;
  }

  auto& operator+=(const HalfEdge e) {
    // The following cast is usually trivial, but may not be when Surface != FlatTriangulation<T>.
    this->vector += static_cast<flatsurf::Vector<T>>(this->surface->fromEdge(e));
    return *this;
  }

  auto& operator+=(const HalfEdgeMap<int>& coefficients) {
    for (auto e : this->surface->halfEdges()) {
      int c = coefficients.get(e);
      if (c > 0) {
        this->vector += c * static_cast<flatsurf::Vector<T>>(this->surface->fromEdge(e));
      }
    }
    return *this;
  }

  operator flatsurf::Vector<T>() const {
    return this->vector;
  }
};
}  // namespace

template <typename T, typename Approximation, typename Surface>
class VectorAlongTriangulation<T, Approximation, Surface>::Implementation : public std::conditional_t<std::is_same_v<Approximation, void>, ImplementationWithForward<VectorAlongTriangulation, Implementation, T>, ImplementationWithApproximation<VectorAlongTriangulation, Implementation, Approximation>> {
  using Base = std::conditional_t<std::is_same_v<Approximation, void>, ImplementationWithForward<VectorAlongTriangulation, Implementation, T>, ImplementationWithApproximation<VectorAlongTriangulation, Implementation, Approximation>>;

 public:
  using Vector = VectorAlongTriangulation<T, Approximation>;
  using Base::Base;

  static Implementation& impl(Vector& v) { return *v.impl; }
  static const Implementation& impl(const Vector& v) { return *v.impl; }
};

template <typename T, typename Approximation, typename Surface>
VectorAlongTriangulation<T, Approximation, Surface>::VectorAlongTriangulation(Surface const* surface)
    : impl(spimpl::make_impl<Implementation>(surface)) {}

template <typename T, typename Approximation, typename Surface>
VectorAlongTriangulation<T, Approximation, Surface>::VectorAlongTriangulation(Surface const* surface, const HalfEdgeMap<int>& coefficients)
    : VectorAlongTriangulation(surface) {
  *this += coefficients;
}

template <typename T, typename Approximation, typename Surface>
VectorAlongTriangulation<T, Approximation, Surface>::operator Vector<T>() const noexcept {
  return static_cast<Vector<T>>(*this->impl);
}

template <typename T, typename Approximation, typename Surface>
VectorAlongTriangulation<T, Approximation, Surface>::VectorAlongTriangulation(Surface const* surface, const std::vector<HalfEdge>& edges) : VectorAlongTriangulation(surface) {
  for (auto edge : edges)
    *this += edge;
}

template <typename T, typename Approximation, typename Surface>
VectorAlongTriangulation<T, Approximation, Surface>& VectorAlongTriangulation<T, Approximation, Surface>::operator+=(HalfEdge e) {
  *impl += e;
  return *this;
}

template <typename T, typename Approximation, typename Surface>
VectorAlongTriangulation<T, Approximation, Surface>& VectorAlongTriangulation<T, Approximation, Surface>::operator-=(HalfEdge e) {
  return *this += -e;
}

template <typename T, typename Approximation, typename Surface>
VectorAlongTriangulation<T, Approximation, Surface>& VectorAlongTriangulation<T, Approximation, Surface>::operator+=(const HalfEdgeMap<int>& coefficients) {
  *impl += coefficients;
  return *this;
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
// (unfortunately, we also need to explicitly instantiate the base classes,
// https://stackoverflow.com/q/3705000/812379).
// Additionally, some C++ instantiation madness is needed, to convince the
// compiler that it does not have to create implementations of Vector<>, which
// it cannot because it cannot see the full type of their Implementation class
// here.
#include <e-antic/renfxx.h>
#include <exact-real/arb.hpp>
#include <exact-real/element.hpp>
#include <exact-real/integer_ring_traits.hpp>
#include <exact-real/number_field_traits.hpp>
#include <exact-real/rational_field_traits.hpp>

namespace flatsurf {
using eantic::renf_elem_class;
using exactreal::Arb;
using exactreal::Element;
using exactreal::IntegerRingTraits;
using exactreal::NumberFieldTraits;
using exactreal::RationalFieldTraits;

extern template VectorBase<Vector<Arb>>::operator std::complex<double>() const noexcept;
extern template Vector<Arb> VectorBase<Vector<Arb>>::operator-() const noexcept;
extern template Vector<Arb>& VectorBase<Vector<Arb>>::operator+=(const Vector<Arb>&);
extern template Vector<Arb>& VectorBase<Vector<Arb>>::operator*=(int);
extern template std::ostream& detail::operator<<(std::ostream&, const VectorBase<Vector<Arb>>&);

// long long
extern template bool VectorExact<Vector<long long>, long long>::operator>(Bound) const noexcept;
extern template bool VectorExact<Vector<long long>, long long>::operator<(Bound) const noexcept;
extern template VectorExact<Vector<long long>, long long>::operator bool() const noexcept;
extern template CCW VectorExact<Vector<long long>, long long>::ccw(const Vector<long long>&) const noexcept;
extern template ORIENTATION VectorExact<Vector<long long>, long long>::orientation(const Vector<long long>&) const noexcept;
template class VectorAlongTriangulation<long long>;
extern template long long VectorExact<Vector<long long>, long long>::x() const noexcept;
extern template long long VectorExact<Vector<long long>, long long>::y() const noexcept;
extern template long long VectorExact<Vector<long long>, long long>::operator*(const Vector<long long>&)const noexcept;
extern template bool VectorExact<Vector<long long>, long long>::operator==(const Vector<long long>&) const noexcept;
template class detail::VectorExact<VectorAlongTriangulation<long long>, long long>;
extern template Vector<long long>& VectorBase<Vector<long long>>::operator+=(const Vector<long long>&);
extern template Vector<long long>& VectorBase<Vector<long long>>::operator*=(int);
extern template Vector<long long>& VectorBase<Vector<long long>>::operator*=(const mpz_class&);
extern template Vector<long long> VectorBase<Vector<long long>>::operator-() const noexcept;
extern template Vector<long long> VectorBase<Vector<long long>>::perpendicular() const;
extern template VectorBase<Vector<long long>>::operator Vector<Arb>() const noexcept;
extern template VectorBase<Vector<long long>>::operator std::complex<double>() const noexcept;
extern template VectorBase<Vector<long long>>::operator Vector<Arb>() const noexcept;
template class detail::VectorBase<VectorAlongTriangulation<long long>>;
extern template std::ostream& detail::operator<<(std::ostream&, const VectorBase<Vector<long long>>&);
template std::ostream& detail::operator<<(std::ostream&, const VectorBase<VectorAlongTriangulation<long long>>&);

// renf_elem_class
extern template bool VectorExact<Vector<renf_elem_class>, renf_elem_class>::operator>(Bound) const noexcept;
extern template bool VectorExact<Vector<renf_elem_class>, renf_elem_class>::operator<(Bound) const noexcept;
extern template VectorExact<Vector<renf_elem_class>, renf_elem_class>::operator bool() const noexcept;
extern template CCW VectorExact<Vector<renf_elem_class>, renf_elem_class>::ccw(const Vector<renf_elem_class>&) const noexcept;
extern template ORIENTATION VectorExact<Vector<renf_elem_class>, renf_elem_class>::orientation(const Vector<renf_elem_class>&) const noexcept;
template class VectorAlongTriangulation<renf_elem_class>;
extern template renf_elem_class VectorExact<Vector<renf_elem_class>, renf_elem_class>::x() const noexcept;
extern template renf_elem_class VectorExact<Vector<renf_elem_class>, renf_elem_class>::y() const noexcept;
extern template renf_elem_class VectorExact<Vector<renf_elem_class>, renf_elem_class>::operator*(const Vector<renf_elem_class>&)const noexcept;
extern template bool VectorExact<Vector<renf_elem_class>, renf_elem_class>::operator==(const Vector<renf_elem_class>&) const noexcept;
template class detail::VectorExact<VectorAlongTriangulation<renf_elem_class>, renf_elem_class>;
extern template Vector<renf_elem_class>& VectorBase<Vector<renf_elem_class>>::operator+=(const Vector<renf_elem_class>&);
extern template Vector<renf_elem_class>& VectorBase<Vector<renf_elem_class>>::operator*=(int);
extern template Vector<renf_elem_class>& VectorBase<Vector<renf_elem_class>>::operator*=(const mpz_class&);
extern template Vector<renf_elem_class> VectorBase<Vector<renf_elem_class>>::operator-() const noexcept;
extern template Vector<renf_elem_class> VectorBase<Vector<renf_elem_class>>::perpendicular() const;
extern template VectorBase<Vector<renf_elem_class>>::operator Vector<Arb>() const noexcept;
extern template VectorBase<Vector<renf_elem_class>>::operator std::complex<double>() const noexcept;
extern template VectorBase<Vector<renf_elem_class>>::operator Vector<Arb>() const noexcept;
template class detail::VectorBase<VectorAlongTriangulation<renf_elem_class>>;
extern template std::ostream& detail::operator<<(std::ostream&, const VectorBase<Vector<renf_elem_class>>&);
template std::ostream& detail::operator<<(std::ostream&, const VectorBase<VectorAlongTriangulation<renf_elem_class>>&);

template class VectorAlongTriangulation<renf_elem_class, Arb>;
template class detail::VectorExact<VectorAlongTriangulation<renf_elem_class, Arb>, renf_elem_class>;
template std::ostream& detail::operator<<(std::ostream&, const VectorBase<VectorAlongTriangulation<renf_elem_class, Arb>>&);

// Element<Integer>
extern template bool VectorExact<Vector<Element<IntegerRingTraits>>, Element<IntegerRingTraits>>::operator>(Bound) const noexcept;
extern template bool VectorExact<Vector<Element<IntegerRingTraits>>, Element<IntegerRingTraits>>::operator<(Bound) const noexcept;
extern template VectorExact<Vector<Element<IntegerRingTraits>>, Element<IntegerRingTraits>>::operator bool() const noexcept;
extern template CCW VectorExact<Vector<Element<IntegerRingTraits>>, Element<IntegerRingTraits>>::ccw(const Vector<Element<IntegerRingTraits>>&) const noexcept;
extern template ORIENTATION VectorExact<Vector<Element<IntegerRingTraits>>, Element<IntegerRingTraits>>::orientation(const Vector<Element<IntegerRingTraits>>&) const noexcept;
template class VectorAlongTriangulation<Element<IntegerRingTraits>>;
extern template Element<IntegerRingTraits> VectorExact<Vector<Element<IntegerRingTraits>>, Element<IntegerRingTraits>>::x() const noexcept;
extern template Element<IntegerRingTraits> VectorExact<Vector<Element<IntegerRingTraits>>, Element<IntegerRingTraits>>::y() const noexcept;
extern template Element<IntegerRingTraits> VectorExact<Vector<Element<IntegerRingTraits>>, Element<IntegerRingTraits>>::operator*(const Vector<Element<IntegerRingTraits>>&)const noexcept;
extern template bool VectorExact<Vector<Element<IntegerRingTraits>>, Element<IntegerRingTraits>>::operator==(const Vector<Element<IntegerRingTraits>>&) const noexcept;
template class detail::VectorExact<VectorAlongTriangulation<Element<IntegerRingTraits>>, Element<IntegerRingTraits>>;
extern template Vector<Element<IntegerRingTraits>>& VectorBase<Vector<Element<IntegerRingTraits>>>::operator+=(const Vector<Element<IntegerRingTraits>>&);
extern template Vector<Element<IntegerRingTraits>>& VectorBase<Vector<Element<IntegerRingTraits>>>::operator*=(int);
extern template Vector<Element<IntegerRingTraits>>& VectorBase<Vector<Element<IntegerRingTraits>>>::operator*=(const mpz_class&);
extern template Vector<Element<IntegerRingTraits>> VectorBase<Vector<Element<IntegerRingTraits>>>::operator-() const noexcept;
extern template Vector<Element<IntegerRingTraits>> VectorBase<Vector<Element<IntegerRingTraits>>>::perpendicular() const;
extern template VectorBase<Vector<Element<IntegerRingTraits>>>::operator Vector<Arb>() const noexcept;
extern template VectorBase<Vector<Element<IntegerRingTraits>>>::operator std::complex<double>() const noexcept;
extern template VectorBase<Vector<Element<IntegerRingTraits>>>::operator Vector<Arb>() const noexcept;
template class detail::VectorBase<VectorAlongTriangulation<Element<IntegerRingTraits>>>;
extern template std::ostream& detail::operator<<(std::ostream&, const VectorBase<Vector<Element<IntegerRingTraits>>>&);
template std::ostream& detail::operator<<(std::ostream&, const VectorBase<VectorAlongTriangulation<Element<IntegerRingTraits>>>&);

template class VectorAlongTriangulation<Element<IntegerRingTraits>, Arb>;
template class detail::VectorExact<VectorAlongTriangulation<Element<IntegerRingTraits>, Arb>, Element<IntegerRingTraits>>;
template std::ostream& detail::operator<<(std::ostream&, const VectorBase<VectorAlongTriangulation<Element<IntegerRingTraits>, Arb>>&);

// Element<Rational>
extern template bool VectorExact<Vector<Element<RationalFieldTraits>>, Element<RationalFieldTraits>>::operator>(Bound) const noexcept;
extern template bool VectorExact<Vector<Element<RationalFieldTraits>>, Element<RationalFieldTraits>>::operator<(Bound) const noexcept;
extern template VectorExact<Vector<Element<RationalFieldTraits>>, Element<RationalFieldTraits>>::operator bool() const noexcept;
extern template CCW VectorExact<Vector<Element<RationalFieldTraits>>, Element<RationalFieldTraits>>::ccw(const Vector<Element<RationalFieldTraits>>&) const noexcept;
extern template ORIENTATION VectorExact<Vector<Element<RationalFieldTraits>>, Element<RationalFieldTraits>>::orientation(const Vector<Element<RationalFieldTraits>>&) const noexcept;
template class VectorAlongTriangulation<Element<RationalFieldTraits>>;
extern template Element<RationalFieldTraits> VectorExact<Vector<Element<RationalFieldTraits>>, Element<RationalFieldTraits>>::x() const noexcept;
extern template Element<RationalFieldTraits> VectorExact<Vector<Element<RationalFieldTraits>>, Element<RationalFieldTraits>>::y() const noexcept;
extern template Element<RationalFieldTraits> VectorExact<Vector<Element<RationalFieldTraits>>, Element<RationalFieldTraits>>::operator*(const Vector<Element<RationalFieldTraits>>&)const noexcept;
extern template bool VectorExact<Vector<Element<RationalFieldTraits>>, Element<RationalFieldTraits>>::operator==(const Vector<Element<RationalFieldTraits>>&) const noexcept;
template class detail::VectorExact<VectorAlongTriangulation<Element<RationalFieldTraits>>, Element<RationalFieldTraits>>;
extern template Vector<Element<RationalFieldTraits>>& VectorBase<Vector<Element<RationalFieldTraits>>>::operator+=(const Vector<Element<RationalFieldTraits>>&);
extern template Vector<Element<RationalFieldTraits>>& VectorBase<Vector<Element<RationalFieldTraits>>>::operator*=(int);
extern template Vector<Element<RationalFieldTraits>>& VectorBase<Vector<Element<RationalFieldTraits>>>::operator*=(const mpz_class&);
extern template Vector<Element<RationalFieldTraits>> VectorBase<Vector<Element<RationalFieldTraits>>>::operator-() const noexcept;
extern template Vector<Element<RationalFieldTraits>> VectorBase<Vector<Element<RationalFieldTraits>>>::perpendicular() const;
extern template VectorBase<Vector<Element<RationalFieldTraits>>>::operator Vector<Arb>() const noexcept;
extern template VectorBase<Vector<Element<RationalFieldTraits>>>::operator std::complex<double>() const noexcept;
extern template VectorBase<Vector<Element<RationalFieldTraits>>>::operator Vector<Arb>() const noexcept;
template class detail::VectorBase<VectorAlongTriangulation<Element<RationalFieldTraits>>>;
extern template std::ostream& detail::operator<<(std::ostream&, const VectorBase<Vector<Element<RationalFieldTraits>>>&);
template std::ostream& detail::operator<<(std::ostream&, const VectorBase<VectorAlongTriangulation<Element<RationalFieldTraits>>>&);

template class VectorAlongTriangulation<Element<RationalFieldTraits>, Arb>;
template class detail::VectorExact<VectorAlongTriangulation<Element<RationalFieldTraits>, Arb>, Element<RationalFieldTraits>>;
template std::ostream& detail::operator<<(std::ostream&, const VectorBase<VectorAlongTriangulation<Element<RationalFieldTraits>, Arb>>&);

// Element<NumberField>
extern template bool VectorExact<Vector<Element<NumberFieldTraits>>, Element<NumberFieldTraits>>::operator>(Bound) const noexcept;
extern template bool VectorExact<Vector<Element<NumberFieldTraits>>, Element<NumberFieldTraits>>::operator<(Bound) const noexcept;
extern template VectorExact<Vector<Element<NumberFieldTraits>>, Element<NumberFieldTraits>>::operator bool() const noexcept;
extern template CCW VectorExact<Vector<Element<NumberFieldTraits>>, Element<NumberFieldTraits>>::ccw(const Vector<Element<NumberFieldTraits>>&) const noexcept;
extern template ORIENTATION VectorExact<Vector<Element<NumberFieldTraits>>, Element<NumberFieldTraits>>::orientation(const Vector<Element<NumberFieldTraits>>&) const noexcept;
template class VectorAlongTriangulation<Element<NumberFieldTraits>>;
extern template Element<NumberFieldTraits> VectorExact<Vector<Element<NumberFieldTraits>>, Element<NumberFieldTraits>>::x() const noexcept;
extern template Element<NumberFieldTraits> VectorExact<Vector<Element<NumberFieldTraits>>, Element<NumberFieldTraits>>::y() const noexcept;
extern template Element<NumberFieldTraits> VectorExact<Vector<Element<NumberFieldTraits>>, Element<NumberFieldTraits>>::operator*(const Vector<Element<NumberFieldTraits>>&)const noexcept;
extern template bool VectorExact<Vector<Element<NumberFieldTraits>>, Element<NumberFieldTraits>>::operator==(const Vector<Element<NumberFieldTraits>>&) const noexcept;
template class detail::VectorExact<VectorAlongTriangulation<Element<NumberFieldTraits>>, Element<NumberFieldTraits>>;
extern template Vector<Element<NumberFieldTraits>>& VectorBase<Vector<Element<NumberFieldTraits>>>::operator+=(const Vector<Element<NumberFieldTraits>>&);
extern template Vector<Element<NumberFieldTraits>>& VectorBase<Vector<Element<NumberFieldTraits>>>::operator*=(int);
extern template Vector<Element<NumberFieldTraits>>& VectorBase<Vector<Element<NumberFieldTraits>>>::operator*=(const mpz_class&);
extern template Vector<Element<NumberFieldTraits>> VectorBase<Vector<Element<NumberFieldTraits>>>::operator-() const noexcept;
extern template Vector<Element<NumberFieldTraits>> VectorBase<Vector<Element<NumberFieldTraits>>>::perpendicular() const;
extern template VectorBase<Vector<Element<NumberFieldTraits>>>::operator Vector<Arb>() const noexcept;
extern template VectorBase<Vector<Element<NumberFieldTraits>>>::operator std::complex<double>() const noexcept;
extern template VectorBase<Vector<Element<NumberFieldTraits>>>::operator Vector<Arb>() const noexcept;
template class detail::VectorBase<VectorAlongTriangulation<Element<NumberFieldTraits>>>;
extern template std::ostream& detail::operator<<(std::ostream&, const VectorBase<Vector<Element<NumberFieldTraits>>>&);
template std::ostream& detail::operator<<(std::ostream&, const VectorBase<VectorAlongTriangulation<Element<NumberFieldTraits>>>&);

template class VectorAlongTriangulation<Element<NumberFieldTraits>, Arb>;
template class detail::VectorExact<VectorAlongTriangulation<Element<NumberFieldTraits>, Arb>, Element<NumberFieldTraits>>;
template std::ostream& detail::operator<<(std::ostream&, const VectorBase<VectorAlongTriangulation<Element<NumberFieldTraits>, Arb>>&);
}  // namespace flatsurf
