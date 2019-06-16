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

#include <exact-real/element.hpp>
#include <exact-real/integer_ring_traits.hpp>
#include <exact-real/number_field_traits.hpp>
#include <exact-real/rational_field_traits.hpp>
#include <exact-real/yap/arb.hpp>

#include "flatsurf/vector.hpp"

#include "algorithm/exact.ipp"
#include "algorithm/with_error.ipp"
#include "storage/cartesian.ipp"

using std::enable_if_t;
using std::is_same_v;

// We currently use this precision for all computations involving Arb.
// This is somewhat random and should probably change, see
// https://github.com/flatsurf/flatsurf/issues/52.
// It would also be conceivable to construct the predicates involving yap
// expressions and have Arb decide automatically, i.e., without any custom code
// here. However, it is a bit unclear whether there is a reasonable choice of
// precision for that.
using exactreal::Arb;
using exactreal::ARB_PRECISION_FAST;

namespace flatsurf {
template <typename T>
class Vector<T>::Implementation : public Cartesian<T> {
 public:
  using Cartesian<T>::Cartesian;
  using Vector = flatsurf::Vector<T>;

  static Vector make(const T& x, const T& y) {
    return Vector(x, y);
  }

  template <typename _ = void, typename = std::enable_if_t<std::is_same_v<T, exactreal::Arb>, void>>
  Implementation& operator+=(const flatsurf::Vector<exactreal::Arb>& rhs) {
    this->x += rhs.impl->x(ARB_PRECISION_FAST);
    this->y += rhs.impl->y(ARB_PRECISION_FAST);
    return *this;
  }

  template <typename S, typename = std::enable_if_t<std::is_same_v<T, long long> || std::is_same_v<T, exactreal::Arb>, void>>
  Implementation& operator*=(const S& c) {
    if constexpr(std::is_same_v<T, long long>) {
      if constexpr(std::is_same_v<S, mpz_class>) {
        if (mpz_fits_sint_p(c.get_mpz_t())) {
          *this *= c.get_si();
        } else {
          throw std::logic_error("not implemented: multiplication with mpz_class.");
        }
      } else {
        this->x *= c;
        this->y *= c;
      }
    } else {
      this->x *= Arb(c)(ARB_PRECISION_FAST);
      this->y *= Arb(c)(ARB_PRECISION_FAST);
    }
    return *this;
  }

  template <typename _ = void, typename = std::enable_if_t<std::is_same_v<T, exactreal::Arb>, void>>
  std::optional<CCW> ccw(const flatsurf::Vector<exactreal::Arb>& rhs) const noexcept {
    const Arb a = (this->x * rhs.impl->y)(ARB_PRECISION_FAST);
    const Arb b = (rhs.impl->x * this->y)(ARB_PRECISION_FAST);

    bool overlaps = arb_overlaps(a.arb_t(), b.arb_t());
    if (overlaps) {
      if (arb_is_exact(a.arb_t()) && arb_is_exact(b.arb_t())) {
        if (a.equal(b)) {
          // a and b are identical single point sets
          return CCW::COLLINEAR;
        }
      }
      return {};
    } else {
      int cmp = arf_cmp(arb_midref(a.arb_t()), arb_midref(b.arb_t()));
      assert(cmp != 0);
      if (cmp < 0)
        return CCW::CLOCKWISE;
      else
        return CCW::COUNTERCLOCKWISE;
    }
  }

  template <typename _ = void, typename = std::enable_if_t<std::is_same_v<T, exactreal::Arb>, void>>
  std::optional<ORIENTATION> orientation(const flatsurf::Vector<exactreal::Arb>& rhs) const noexcept {
    // Arb also has a built-in dot product. It's probably not doing anything else in 2d.
    const Arb dot = (this->x * rhs.impl->x + this->y * rhs.impl->y)(ARB_PRECISION_FAST);

    auto cmp = dot > 0;
    if (cmp.has_value()) {
      if (*cmp) {
        return ORIENTATION::SAME;
      } else {
        auto is_zero = dot == Arb();
        if (is_zero.has_value() && *is_zero) {
          // dot is the single point 0 without any ball imprecision
          return ORIENTATION::ORTHOGONAL;
        } else {
          return ORIENTATION::OPPOSITE;
        }
      }
    }
    return {};
  }

  template <typename _ = void, typename = std::enable_if_t<std::is_same_v<T, exactreal::Arb>, void>>
  std::optional<bool> operator<(const Bound bound) const noexcept {
    Arb size = (this->x * this->x + this->y * this->y)(ARB_PRECISION_FAST);
    return size < bound.squared;
  }

  template <typename _ = void, typename = std::enable_if_t<std::is_same_v<T, exactreal::Arb>, void>>
  std::optional<bool> operator>(const Bound bound) const noexcept {
    Arb size = (this->x * this->x + this->y * this->y)(ARB_PRECISION_FAST);
    return size > bound.squared;
  }

  template <typename = void, typename = std::enable_if_t<std::is_same_v<T, exactreal::Arb>, void>>
  operator std::optional<bool>() const noexcept {
    auto maybe_x = this->x == Arb(0);
    if (!maybe_x || !*maybe_x)
      return maybe_x;
    auto maybe_y = this->y == Arb(0);
    if (!maybe_y || !*maybe_y)
      return maybe_y;

    return true;
  }

  template <typename = void, typename = std::enable_if_t<std::is_same_v<T, exactreal::Arb>, void>>
  Vector projection(const Vector& rhs) const {
    Arb dot = *this * rhs;
    return make((dot * rhs.impl->x)(ARB_PRECISION_FAST),
                (dot * rhs.impl->y)(ARB_PRECISION_FAST));
  }

  template <typename = void, typename = std::enable_if_t<std::is_same_v<T, Arb>, void>>
  Arb operator*(const Vector& rhs) const {
    return (this->x * rhs.impl->x + this->y * rhs.impl->y)(ARB_PRECISION_FAST);
  }

  template <typename = void, typename = std::enable_if_t<std::is_same_v<T, eantic::renf_elem_class> || std::is_same_v<T, exactreal::Element<exactreal::IntegerRingTraits>> || std::is_same_v<T, exactreal::Element<exactreal::RationalFieldTraits>> || std::is_same_v<T, exactreal::Element<exactreal::NumberFieldTraits>>, void>>
  operator flatsurf::Vector<exactreal::Arb>() const noexcept {
    if constexpr (is_same_v<T, eantic::renf_elem_class>) {
      return flatsurf::Vector<exactreal::Arb>(Arb(this->x, ARB_PRECISION_FAST), Arb(this->y, ARB_PRECISION_FAST));
    } else {
      return flatsurf::Vector<exactreal::Arb>(this->x.arb(ARB_PRECISION_FAST), this->y.arb(ARB_PRECISION_FAST));
    }
  }
};

template <typename T>
Vector<T>::Vector() : impl(spimpl::make_impl<Implementation>(T(), T())) {}

template <typename T>
Vector<T>::Vector(const T& x, const T& y) : impl(spimpl::make_impl<Implementation>(x, y)) {}
}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
// (unfortunately, we also need to explicitly instantiate the base classes,
// https://stackoverflow.com/q/3705000/812379).
#include <e-antic/renfxx.h>
#include <exact-real/arb.hpp>

namespace flatsurf {
using eantic::renf_elem_class;
using exactreal::Arb;
using exactreal::Element;
using exactreal::IntegerRingTraits;
using exactreal::NumberFieldTraits;
using exactreal::RationalFieldTraits;

template class Vector<Arb>;
template class Vector<long long>;
template class Vector<renf_elem_class>;
template class Vector<Element<IntegerRingTraits>>;
template class Vector<Element<RationalFieldTraits>>;
template class Vector<Element<NumberFieldTraits>>;

namespace detail {
template class VectorWithError<Vector<Arb>>;
template class VectorBase<Vector<Arb>>;
template std::ostream& operator<<(std::ostream&, const VectorBase<Vector<Arb>>&);

template class VectorExact<Vector<long long>, long long>;
template class VectorBase<Vector<long long>>;
template std::ostream& operator<<(std::ostream&, const VectorBase<Vector<long long>>&);

template class VectorExact<Vector<renf_elem_class>, renf_elem_class>;
template class VectorBase<Vector<renf_elem_class>>;
template std::ostream& operator<<(std::ostream&, const VectorBase<Vector<renf_elem_class>>&);

template class VectorExact<Vector<Element<IntegerRingTraits>>, Element<IntegerRingTraits>>;
template class VectorBase<Vector<Element<IntegerRingTraits>>>;
template std::ostream& operator<<(std::ostream&, const VectorBase<Vector<Element<IntegerRingTraits>>>&);

template class VectorExact<Vector<Element<RationalFieldTraits>>, Element<RationalFieldTraits>>;
template class VectorBase<Vector<Element<RationalFieldTraits>>>;
template std::ostream& operator<<(std::ostream&, const VectorBase<Vector<Element<RationalFieldTraits>>>&);

template class VectorExact<Vector<Element<NumberFieldTraits>>, Element<NumberFieldTraits>>;
template class VectorBase<Vector<Element<NumberFieldTraits>>>;
template std::ostream& operator<<(std::ostream&, const VectorBase<Vector<Element<NumberFieldTraits>>>&);
}  // namespace detail
}  // namespace flatsurf
