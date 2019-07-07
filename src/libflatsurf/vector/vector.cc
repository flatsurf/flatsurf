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

#include <boost/lexical_cast.hpp>
#include <exact-real/element.hpp>
#include <exact-real/integer_ring_traits.hpp>
#include <exact-real/number_field_traits.hpp>
#include <exact-real/rational_field_traits.hpp>
#include <exact-real/yap/arb.hpp>

#include "flatsurf/vector.hpp"

#include "../util/assert.ipp"
#include "algorithm/exact.ipp"
#include "algorithm/with_error.ipp"
#include "storage/cartesian.ipp"

using boost::lexical_cast;

// We currently use this precision for all computations involving Arb.
// This is somewhat random and should probably change, see
// https://github.com/flatsurf/flatsurf/issues/52.
// It would also be conceivable to construct the predicates involving yap
// expressions and have Arb decide automatically, i.e., without any custom code
// here. However, it is a bit unclear whether there is a reasonable choice of
// precision for that.
using exactreal::Arb;
using exactreal::ARB_PRECISION_FAST;

template <bool Condition>
using If = std::enable_if_t<Condition, bool>;

template <typename S, typename T>
inline constexpr bool Similar = std::is_same_v<std::decay_t<S>, std::decay_t<T>>;

template <typename T>
inline constexpr bool IsArb = Similar<T, Arb>;

template <typename T>
inline constexpr bool IsExactReal = Similar<T, exactreal::Element<exactreal::IntegerRingTraits>> || Similar<T, exactreal::Element<exactreal::RationalFieldTraits>> || Similar<T, exactreal::Element<exactreal::NumberFieldTraits>>;

template <typename T>
inline constexpr bool IsEAntic = Similar<T, eantic::renf_elem_class>;

template <typename T>
inline constexpr bool IsMPZ = Similar<T, mpz_class>;

template <typename T>
inline constexpr bool IsMPQ = Similar<T, mpq_class>;

template <typename T>
inline constexpr bool IsLongLong = Similar<T, long long>;

namespace flatsurf {
template <typename T>
class Vector<T>::Implementation : public Cartesian<T> {
 public:
  using Cartesian<T>::Cartesian;
  using Vector = flatsurf::Vector<T>;

  static Vector make(const T& x, const T& y) {
    return Vector(x, y);
  }

  template <bool Enable = IsArb<T>, If<Enable> = true>
  Implementation& operator+=(const flatsurf::Vector<Arb>& rhs) {
    this->x += rhs.impl->x(ARB_PRECISION_FAST);
    this->y += rhs.impl->y(ARB_PRECISION_FAST);
    return *this;
  }

  template <typename S, bool Enable = IsArb<T>, If<Enable> = true>
  Implementation& operator*=(const S& rhs) {
    this->x *= Arb(rhs)(ARB_PRECISION_FAST);
    this->y *= Arb(rhs)(ARB_PRECISION_FAST);
    return *this;
  }

  template <typename S, bool Enable = IsLongLong<T>&& IsMPZ<S>, If<Enable> = true, typename = void>
  Implementation& operator*=(const S& rhs) {
    assert(rhs * mpz_class(lexical_cast<std::string>(this->x)) <= mpz_class(lexical_cast<std::string>(LONG_LONG_MAX)) && "Multiplication overflow");
    assert(rhs * mpz_class(lexical_cast<std::string>(this->y)) <= mpz_class(lexical_cast<std::string>(LONG_LONG_MAX)) && "Multiplication overflow");
    this->x *= lexical_cast<long long>(lexical_cast<std::string>(rhs));
    this->y *= lexical_cast<long long>(lexical_cast<std::string>(rhs));
    return *this;
  }

  template <bool Enable = IsArb<T>, If<Enable> = true>
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

  template <bool Enable = IsArb<T>, If<Enable> = true>
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

  template <bool Enable = IsArb<T>, If<Enable> = true>
  std::optional<bool> operator<(const Bound bound) const noexcept {
    Arb size = (this->x * this->x + this->y * this->y)(ARB_PRECISION_FAST);
    return size < bound.length() * bound.length();
  }

  template <bool Enable = IsMPZ<T> || IsMPQ<T>, If<Enable> = true, typename = void>
  bool operator<(const Bound bound) const noexcept {
    return this->x * this->x + this->y * this->y < mpz_class(boost::lexical_cast<std::string>(bound.length() * bound.length()));
  }

  template <bool Enable = IsArb<T>, If<Enable> = true>
  std::optional<bool> operator>(const Bound bound) const noexcept {
    Arb size = (this->x * this->x + this->y * this->y)(ARB_PRECISION_FAST);
    return size > bound.length() * bound.length();
  }

  template <bool Enable = IsMPZ<T> || IsMPQ<T>, If<Enable> = true, typename = void>
  bool operator>(const Bound bound) const noexcept {
    return this->x * this->x + this->y * this->y > mpz_class(boost::lexical_cast<std::string>(bound.length() * bound.length()));
  }

  template <bool Enable = IsArb<T>, If<Enable> = true>
  operator std::optional<bool>() const noexcept {
    auto maybe_x = this->x == Arb(0);
    if (!maybe_x || !*maybe_x)
      return maybe_x;
    auto maybe_y = this->y == Arb(0);
    if (!maybe_y || !*maybe_y)
      return maybe_y;

    return true;
  }

  template <bool Enable = IsArb<T>, If<Enable> = true>
  Vector projection(const Vector& rhs) const {
    Arb dot = *this * rhs;
    return make((dot * rhs.impl->x)(ARB_PRECISION_FAST),
                (dot * rhs.impl->y)(ARB_PRECISION_FAST));
  }

  template <bool Enable = IsArb<T>, If<Enable> = true>
  Arb operator*(const Vector& rhs) const {
    return (this->x * rhs.impl->x + this->y * rhs.impl->y)(ARB_PRECISION_FAST);
  }

  template <bool Enable = IsEAntic<T> || IsMPQ<T>, If<Enable> = true>
  operator flatsurf::Vector<exactreal::Arb>() const noexcept {
    return flatsurf::Vector<exactreal::Arb>(Arb(this->x, ARB_PRECISION_FAST), Arb(this->y, ARB_PRECISION_FAST));
  }

  template <bool Enable = IsExactReal<T>, If<Enable> = true, typename = void>
  operator flatsurf::Vector<exactreal::Arb>() const noexcept {
    return flatsurf::Vector<exactreal::Arb>(this->x.arb(ARB_PRECISION_FAST), this->y.arb(ARB_PRECISION_FAST));
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
template class Vector<mpz_class>;
template class Vector<mpq_class>;
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

template class VectorExact<Vector<mpz_class>, mpz_class>;
template class VectorBase<Vector<mpz_class>>;
template std::ostream& operator<<(std::ostream&, const VectorBase<Vector<mpz_class>>&);

template class VectorExact<Vector<mpq_class>, mpq_class>;
template class VectorBase<Vector<mpq_class>>;
template std::ostream& operator<<(std::ostream&, const VectorBase<Vector<mpq_class>>&);

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
