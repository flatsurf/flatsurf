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
#include <exact-real/integer_ring.hpp>
#include <exact-real/number_field.hpp>
#include <exact-real/rational_field.hpp>
#include <exact-real/yap/arb.hpp>
#include <exact-real/arf.hpp>

#include "../../flatsurf/vector.hpp"

#include "../util/assert.ipp"
#include "../util/hash.ipp"

#include "../external/gmpxxll/mpz_class.hpp"

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
using exactreal::Arf;
using exactreal::ARB_PRECISION_FAST;

template <bool Condition>
using If = std::enable_if_t<Condition, bool>;

template <typename S, typename T>
inline constexpr bool Similar = std::is_same_v<std::decay_t<S>, std::decay_t<T>>;

template <typename T>
inline constexpr bool IsArb = Similar<T, Arb>;

template <typename T>
inline constexpr bool IsExactReal = Similar<T, exactreal::Element<exactreal::IntegerRing>> || Similar<T, exactreal::Element<exactreal::RationalField>> || Similar<T, exactreal::Element<exactreal::NumberField>>;

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
class Implementation<Vector<T>> : public Cartesian<T> {
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
    using gmpxxll::mpz_class;
    ASSERT(rhs * mpz_class(this->x) <= mpz_class(LONG_LONG_MAX), "Multiplication overflow");
    ASSERT(rhs * mpz_class(this->y) <= mpz_class(LONG_LONG_MAX), "Multiplication overflow");
    this->x *= static_cast<long long>(mpz_class(rhs));
    this->y *= static_cast<long long>(mpz_class(rhs));
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
    return size < bound.squared();
  }

  template <bool Enable = IsArb<T>, If<Enable> = true>
  std::optional<bool> operator>(const Bound bound) const noexcept {
    Arb size = (this->x * this->x + this->y * this->y)(ARB_PRECISION_FAST);
    return size > bound.squared();
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

template <typename T>
typename Vector<T>::Coordinate Vector<T>::x() const noexcept { return impl->x; }

template <typename T>
typename Vector<T>::Coordinate Vector<T>::y() const noexcept { return impl->y; }

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& self) {
  using Implementation = typename Vector<T>::Implementation;
  const Vector<T>& s = static_cast<const Vector<T>&>(self);

  if constexpr (has_ostream_lshift<Implementation>) {
    return os << s.impl;
  } else if constexpr (is_forward_v<Implementation>) {
    return os << s.impl->vector;
  } else if constexpr (has_approximation_v<Implementation>) {
    return os << s.impl->approximation();
  } else if constexpr (is_cartesian_v<Implementation>) {
    return os << "(" << s.impl->x << ", " << s.impl->y << ")";
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing operator<<.");
  }
}
}  // namespace flatsurf

namespace std {

using namespace flatsurf;

template <typename T>
size_t hash<Vector<T>>::operator()(const Vector<T>&) const noexcept {
  return 0;
  // TODO: Teach coefficients how to hash.
  // return hash_combine(self.x(), self.y());
}

}

// Instantiations of templates so implementations are generated for the linker
// (unfortunately, we also need to explicitly instantiate the base classes,
// https://stackoverflow.com/q/3705000/812379).
#include "util/instantiate.ipp"

#define LIBFLATSURF_INSTANTIATE_THIS(T) \
  LIBFLATSURF_INSTANTIATE_HASH((Vector<T>)) \
  LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION((Vector<T>)) \
  namespace flatsurf::detail { \
    template class VectorExact<Vector<T>, T>; \
    template class VectorBase<Vector<T>>; \
  }

LIBFLATSURF_INSTANTIATE_MANY((LIBFLATSURF_INSTANTIATE_THIS), LIBFLATSURF_REAL_TYPES)

template class ::flatsurf::Vector<Arb>;
template class ::flatsurf::detail::VectorWithError<::flatsurf::Vector<Arb>>;
template class ::flatsurf::detail::VectorBase<::flatsurf::Vector<Arb>>;
template std::ostream& ::flatsurf::operator<<(std::ostream&, const ::flatsurf::Vector<Arb>&);
