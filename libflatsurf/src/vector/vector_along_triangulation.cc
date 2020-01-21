/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
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

#include "gmpxx.h"

#include "../../flatsurf/flat_triangulation.hpp"
#include "../../flatsurf/flat_triangulation_combinatorial.hpp"
#include "../../flatsurf/half_edge.hpp"
#include "../../flatsurf/edge.hpp"
#include "../../flatsurf/half_edge_map.hpp"
#include "../../flatsurf/vector.hpp"
#include "../../flatsurf/vector_along_triangulation.hpp"

#include "algorithm/exact.ipp"
#include "storage/forward.ipp"

using std::ostream;

namespace flatsurf {
namespace {

template <typename Vector, typename Implementation>
class SharedImplementation {
 public:
  using Surface = typename Vector::Surface;

  explicit SharedImplementation(const std::shared_ptr<const Surface> surface) : surface(surface) {}

  std::shared_ptr<const Surface> surface;
};

template <typename Vector, typename Implementation, typename Approximation>
class ImplementationWithApproximation : SharedImplementation<Vector, Implementation> {
  static void updateAfterFlip(HalfEdgeMap<mpz_class>& map, HalfEdge flip) {
    const auto& parent = map.parent();
    map.set(-parent.nextAtVertex(flip), map.get(flip));
    map.set(-parent.nextInFace(parent.nextInFace(flip)), map.get(flip));
    map.set(flip, 0);
  }

 public:
  using Exact = flatsurf::Vector<typename Vector::Coordinate>;
  using Shared = SharedImplementation<Vector, Implementation>;
  using Surface = typename Shared::Surface;

  ImplementationWithApproximation(const std::shared_ptr<const Surface>& surface) :
    Shared(surface),
    coefficients(surface.get(), [](HalfEdge) { return 0; }, updateAfterFlip),
    approx(surface) {}

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

  auto& operator+=(const HalfEdgeMap<mpz_class>& coefficients) {
    approx += coefficients;
    for (auto e : this->surface->halfEdges()) {
      auto c = coefficients.get(e);
      if (c > 0) {
        this->coefficients.set(e, this->coefficients.get(e) + c);
      }
    }
    return *this;
  }

 private:
  HalfEdgeMap<mpz_class> coefficients;
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
    if constexpr (std::is_same_v<T, exactreal::Arb>) {
      this->vector += this->surface->fromEdgeApproximate(e);
    } else {
      this->vector += this->surface->fromEdge(e);
    }
    return *this;
  }

  auto& operator+=(const HalfEdgeMap<int>& coefficients) {
    for (auto e : this->surface->halfEdges()) {
      int c = coefficients.get(e);
      if (c > 0) {
        if constexpr (std::is_same_v<T, exactreal::Arb>) {
          this->vector += c * this->surface->fromEdgeApproximate(e);
        } else {
          this->vector += c * this->surface->fromEdge(e);
        }
      }
    }
    return *this;
  }

  auto& operator+=(const HalfEdgeMap<mpz_class>& coefficients) {
    for (auto e : this->surface->halfEdges()) {
      auto c = coefficients.get(e);
      if (c > 0) {
        if constexpr (std::is_same_v<T, exactreal::Arb>) {
          this->vector += c * this->surface->fromEdgeApproximate(e);
        } else {
          this->vector += c * this->surface->fromEdge(e);
        }
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
VectorAlongTriangulation<T, Approximation, Surface>::VectorAlongTriangulation(const std::shared_ptr<const Surface>& surface)
    : impl(spimpl::make_impl<Implementation>(surface)) {}

template <typename T, typename Approximation, typename Surface>
VectorAlongTriangulation<T, Approximation, Surface>::VectorAlongTriangulation(const std::shared_ptr<const Surface>& surface, const HalfEdgeMap<int>& coefficients)
    : VectorAlongTriangulation(surface) {
  *this += coefficients;
}

template <typename T, typename Approximation, typename Surface>
VectorAlongTriangulation<T, Approximation, Surface>::VectorAlongTriangulation(const std::shared_ptr<const Surface>& surface, const HalfEdgeMap<mpz_class>& coefficients)
    : VectorAlongTriangulation(surface) {
  *this += coefficients;
}

template <typename T, typename Approximation, typename Surface>
VectorAlongTriangulation<T, Approximation, Surface>::operator Vector<T>() const noexcept {
  return static_cast<Vector<T>>(*this->impl);
}

template <typename T, typename Approximation, typename Surface>
VectorAlongTriangulation<T, Approximation, Surface>::VectorAlongTriangulation(const std::shared_ptr<const Surface>& surface, const std::vector<HalfEdge>& edges) : VectorAlongTriangulation(surface) {
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

template <typename T, typename Approximation, typename Surface>
VectorAlongTriangulation<T, Approximation, Surface>& VectorAlongTriangulation<T, Approximation, Surface>::operator+=(const HalfEdgeMap<mpz_class>& coefficients) {
  *impl += coefficients;
  return *this;
}

template <typename T, typename Approximation, typename Surface>
std::ostream& operator<<(std::ostream&, const VectorAlongTriangulation<T, Approximation, Surface>&) {
  throw std::logic_error("not implemented: os << VectorAlongTriangulation");
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
// (unfortunately, we also need to explicitly instantiate the base classes,
// https://stackoverflow.com/q/3705000/812379).
// Additionally, some C++ instantiation madness is needed, to convince the
// compiler that it does not have to create implementations of Vector<>, which
// it cannot because it cannot see the full type of their Implementation class
// here.

#include "../util/instantiate.ipp"

namespace flatsurf {

using eantic::renf_elem_class;
using exactreal::Arb;
using exactreal::Element;
using exactreal::IntegerRing;
using exactreal::NumberField;
using exactreal::RationalField;

extern template VectorBase<Vector<Arb>>::operator std::complex<double>() const noexcept;
extern template Vector<Arb> VectorBase<Vector<Arb>>::operator-() const noexcept;
extern template Vector<Arb>& VectorBase<Vector<Arb>>::operator+=(const Vector<Arb>&);
extern template Vector<Arb>& VectorBase<Vector<Arb>>::operator*=(int);
extern template Vector<Arb>& VectorBase<Vector<Arb>>::operator*=(const mpz_class&);

}

#define LIBFLATSURF_INSTANTIATE_THIS(T) \
  LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION((VectorAlongTriangulation<T>)) \
  LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION((VectorAlongTriangulation<T, Arb>)) \
  namespace flatsurf::detail { \
    template class VectorExact<VectorAlongTriangulation<T>, T>; \
    template class VectorExact<VectorAlongTriangulation<T, Arb>, T>; \
  } \
  namespace flatsurf { \
    extern template bool VectorExact<Vector<T>, T>::operator>(Bound) const noexcept; \
    extern template bool VectorExact<Vector<T>, T>::operator<(Bound) const noexcept; \
    extern template VectorExact<Vector<T>, T>::operator bool() const noexcept; \
    extern template CCW VectorExact<Vector<T>, T>::ccw(const Vector<T>&) const noexcept; \
    extern template ORIENTATION VectorExact<Vector<T>, T>::orientation(const Vector<T>&) const noexcept; \
    extern template T VectorExact<Vector<T>, T>::x() const noexcept; \
    extern template T VectorExact<Vector<T>, T>::y() const noexcept; \
    extern template T VectorExact<Vector<T>, T>::operator*(const Vector<T>&)const noexcept; \
    extern template bool VectorExact<Vector<T>, T>::operator==(const Vector<T>&) const noexcept; \
    extern template Vector<T>& VectorBase<Vector<T>>::operator+=(const Vector<T>&); \
    extern template Vector<T>& VectorBase<Vector<T>>::operator*=(int); \
    extern template Vector<T>& VectorBase<Vector<T>>::operator*=(const mpz_class&); \
    extern template Vector<T> VectorBase<Vector<T>>::operator-() const noexcept; \
    extern template Vector<T> VectorBase<Vector<T>>::perpendicular() const; \
    extern template VectorBase<Vector<T>>::operator Vector<Arb>() const noexcept; \
    extern template VectorBase<Vector<T>>::operator std::complex<double>() const noexcept; \
    extern template VectorBase<Vector<T>>::operator Vector<Arb>() const noexcept; \
  }

LIBFLATSURF_INSTANTIATE_MANY((LIBFLATSURF_INSTANTIATE_THIS), LIBFLATSURF_REAL_TYPES)
