/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2020 Julian Rüth
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

#include "../flatsurf/vector.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <boost/type_traits/is_detected.hpp>
#include <boost/type_traits/is_detected_exact.hpp>
#include <exact-real/arb.hpp>
#include <exact-real/arf.hpp>
#include <exact-real/element.hpp>
#include <exact-real/integer_ring.hpp>
#include <exact-real/number_field.hpp>
#include <exact-real/rational_field.hpp>
#include <exact-real/yap/arb.hpp>
#include <gmpxxll/mpz_class.hpp>
#include <ostream>
#include <limits>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/ccw.hpp"
#include "../flatsurf/fmt.hpp"
#include "../flatsurf/orientation.hpp"
#include "impl/approximation.hpp"
#include "impl/vector.impl.hpp"
#include "util/assert.ipp"
#include "util/hash.ipp"

namespace flatsurf {

// We currently use this precision for all computations involving Arb.
// This is somewhat random and should probably change, see
// https://github.com/flatsurf/flatsurf/issues/52.
// It would also be conceivable to construct the predicates involving yap
// expressions and have Arb decide automatically, i.e., without any custom code
// here. However, it is a bit unclear whether there is a reasonable choice of
// precision for that.
using exactreal::ARB_PRECISION_FAST;

template <typename S, typename T>
inline constexpr bool Similar = std::is_same_v<std::decay_t<S>, std::decay_t<T>>;

template <typename T>
inline constexpr bool IsArb = Similar<T, exactreal::Arb>;

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

template <typename T>
using binary_inplace_div_int_t = decltype(std::declval<T>() /= std::declval<int>());
template <typename T>
static constexpr bool has_binary_inplace_div_int = boost::is_detected_v<binary_inplace_div_int_t, T>;

template <typename T>
using binary_inplace_div_mpz_t = decltype(std::declval<T>() /= std::declval<mpz_class>());
template <typename T>
static constexpr bool has_binary_inplace_div_mpz = boost::is_detected_v<binary_inplace_div_mpz_t, T>;

template <typename T>
Vector<T>::Vector() noexcept :
  self(spimpl::make_impl<ImplementationOf<Vector>>()) {}

template <typename T>
Vector<T>::Vector(const T& x, const T& y) :
  self(spimpl::make_impl<ImplementationOf<Vector>>(x, y)) {}

template <typename T>
Vector<T>::Vector(T&& x, T&& y) :
  self(spimpl::make_impl<ImplementationOf<Vector>>(std::move(x), std::move(y))) {}

template <typename T>
T Vector<T>::x() const { return self->x; }

template <typename T>
T Vector<T>::y() const { return self->y; }

template <typename Vector>
Vector& detail::VectorBase<Vector>::operator+=(const Vector& rhs) {
  using T = typename Vector::Coordinate;
  Vector& self = static_cast<Vector&>(*this);

  if constexpr (IsArb<T>) {
    // Unfortunately, exact-real's arb::yap adds too much overhead, in
    // particular in DEBUG builds for this operation that is called all the
    // time. So we have to call arb_add directly here.

    // this->x += rhs.self->x(ARB_PRECISION_FAST);
    arb_add(self.self->x.arb_t(), self.self->x.arb_t(), rhs.self->x.arb_t(), ARB_PRECISION_FAST);

    // this->y += rhs.self->y(ARB_PRECISION_FAST);
    arb_add(self.self->y.arb_t(), self.self->y.arb_t(), rhs.self->y.arb_t(), ARB_PRECISION_FAST);
  } else {
    self.self->x += rhs.self->x;
    self.self->y += rhs.self->y;
  }

  return self;
}

template <typename Vector>
Vector& detail::VectorBase<Vector>::operator-=(const Vector& rhs) {
  return *this += -rhs;
}

template <typename Vector>
Vector detail::VectorBase<Vector>::operator-() const {
  const Vector& self = static_cast<const Vector&>(*this);

  return Vector(-self.self->x, -self.self->y);
}

template <typename Vector>
Vector& detail::VectorBase<Vector>::operator*=(const int rhs) {
  using T = typename Vector::Coordinate;
  Vector& self = static_cast<Vector&>(*this);

  if constexpr (IsArb<T>) {
    arb_mul_si(self.self->x.arb_t(), self.self->x.arb_t(), rhs, ARB_PRECISION_FAST);
    arb_mul_si(self.self->y.arb_t(), self.self->y.arb_t(), rhs, ARB_PRECISION_FAST);
  } else {
    self.self->x *= rhs;
    self.self->y *= rhs;
  }

  return self;
}

template <typename Vector>
Vector& detail::VectorBase<Vector>::operator*=(const mpz_class& rhs) {
  using T = typename Vector::Coordinate;
  Vector& self = static_cast<Vector&>(*this);

  if constexpr (IsArb<T>) {
    self.self->x *= exactreal::Arb(rhs)(ARB_PRECISION_FAST);
    self.self->y *= exactreal::Arb(rhs)(ARB_PRECISION_FAST);
  } else if constexpr (IsLongLong<T>) {
    using gmpxxll::mpz_class;
    LIBFLATSURF_ASSERT(rhs * mpz_class(self.self->x) <= mpz_class(std::numeric_limits<long long>::max()), "Multiplication overflow");
    LIBFLATSURF_ASSERT(rhs * mpz_class(self.self->y) <= mpz_class(std::numeric_limits<long long>::max()), "Multiplication overflow");
    self.self->x *= mpz_class(rhs).get_sll();
    self.self->y *= mpz_class(rhs).get_sll();
  } else {
    self.self->x *= rhs;
    self.self->y *= rhs;
  }

  return self;
}

template <typename Vector>
Vector& detail::VectorBase<Vector>::operator/=(const int rhs) {
  using T = typename Vector::Coordinate;
  Vector& self = static_cast<Vector&>(*this);

  if constexpr (IsArb<T>) {
    arb_div_si(self.self->x.arb_t(), self.self->x.arb_t(), rhs, ARB_PRECISION_FAST);
    arb_div_si(self.self->y.arb_t(), self.self->y.arb_t(), rhs, ARB_PRECISION_FAST);
  } else if constexpr (IsLongLong<T> || has_binary_inplace_div_int<T>) {
    // Strangely, we need to explicitly check for long long here since
    // has_binary_inplace_div_int does not work for long long.
    self.self->x /= rhs;
    self.self->y /= rhs;
  } else {
    (void)rhs;
    throw std::logic_error("not implemented: no suitable division operator implemented");
  }

  return self;
}

template <typename Vector>
Vector& detail::VectorBase<Vector>::operator/=(const mpz_class& rhs) {
  using T = typename Vector::Coordinate;
  Vector& self = static_cast<Vector&>(*this);

  if constexpr (IsArb<T>) {
    self.self->x /= exactreal::Arb(rhs)(ARB_PRECISION_FAST);
    self.self->y /= exactreal::Arb(rhs)(ARB_PRECISION_FAST);
  } else if constexpr (IsLongLong<T>) {
    using gmpxxll::mpz_class;
    if (rhs >= mpz_class(std::numeric_limits<long long>::max())) {
      self.self->x = 0;
      self.self->y = 0;
    } else {
      self.self->x /= mpz_class(rhs).get_sll();
      self.self->y /= mpz_class(rhs).get_sll();
    }
  } else if constexpr (has_binary_inplace_div_mpz<T>) {
    self.self->x /= rhs;
    self.self->y /= rhs;
  } else {
    (void)rhs;
    throw std::logic_error("not implemented: no suitable division operator implemented");
  }

  return self;
}

template <typename Vector>
Vector detail::VectorBase<Vector>::perpendicular() const {
  const Vector& self = static_cast<const Vector&>(*this);

  return Vector(-self.self->y, self.self->x);
}

template <typename Vector>
detail::VectorBase<Vector>::operator std::complex<double>() const {
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (std::is_same_v<Vector, flatsurf::Vector<exactreal::Arb>>) {
    return std::complex<double>(static_cast<double>(self.self->x), static_cast<double>(self.self->y));
  } else {
    return static_cast<std::complex<double>>(static_cast<flatsurf::Vector<exactreal::Arb>>(self));
  }
}

template <typename Vector>
detail::VectorBase<Vector>::operator flatsurf::Vector<exactreal::Arb>() const {
  using T = typename Vector::Coordinate;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (std::is_same_v<Vector, flatsurf::Vector<exactreal::Arb>>) {
    return self;
  } else {
    return flatsurf::Vector<exactreal::Arb>(Approximation<T>::arb(self.self->x), Approximation<T>::arb(self.self->y));
  }
}

template <typename Vector>
std::optional<CCW> detail::VectorWithError<Vector>::ccw(const Vector& rhs) const {
  const Vector& self = static_cast<const Vector&>(*this);

  const exactreal::Arb a = (self.self->x * rhs.self->y)(ARB_PRECISION_FAST);
  const exactreal::Arb b = (rhs.self->x * self.self->y)(ARB_PRECISION_FAST);

  const bool overlaps = arb_overlaps(a.arb_t(), b.arb_t());
  if (overlaps) {
    if (arb_is_exact(a.arb_t()) && arb_is_exact(b.arb_t())) {
      if (a.equal(b)) {
        // a and b are identical single point sets
        return CCW::COLLINEAR;
      }
    }
    return std::nullopt;
  } else {
    int cmp = arf_cmp(arb_midref(a.arb_t()), arb_midref(b.arb_t()));
    assert(cmp != 0);
    if (cmp < 0)
      return CCW::CLOCKWISE;
    else
      return CCW::COUNTERCLOCKWISE;
  }
}

template <typename Vector>
std::optional<ORIENTATION> detail::VectorWithError<Vector>::orientation(const Vector& rhs) const {
  const Vector& self = static_cast<const Vector&>(*this);

  // Arb also has a built-in dot product. It's probably not doing anything else in 2d.
  const exactreal::Arb dot = (self.self->x * rhs.self->x + self.self->y * rhs.self->y)(ARB_PRECISION_FAST);

  auto cmp = dot > 0;
  if (cmp.has_value()) {
    if (*cmp) {
      return ORIENTATION::SAME;
    } else {
      auto is_zero = dot == exactreal::Arb();
      if (is_zero.has_value() && *is_zero) {
        // dot is the single point 0 without any ball imprecision
        return ORIENTATION::ORTHOGONAL;
      } else {
        return ORIENTATION::OPPOSITE;
      }
    }
  }
  return std::nullopt;
}

template <typename Vector>
std::optional<bool> detail::VectorWithError<Vector>::operator>(Bound bound) const {
  const Vector& self = static_cast<const Vector&>(*this);

  if (!bound) {
    auto nonzero = static_cast<std::optional<bool>>(*this);
    if (nonzero) return *nonzero;
  }

  exactreal::Arb size = (self.self->x * self.self->x + self.self->y * self.self->y)(ARB_PRECISION_FAST);
  return size > bound.squared();
}

template <typename Vector>
std::optional<bool> detail::VectorWithError<Vector>::operator>=(Bound bound) const {
  const auto lt = *this < bound;

  if (!lt)
    return std::nullopt;

  return !*lt;
}

template <typename Vector>
std::optional<bool> detail::VectorWithError<Vector>::operator<(Bound bound) const {
  const Vector& self = static_cast<const Vector&>(*this);

  if (!bound) return false;
  exactreal::Arb size = (self.self->x * self.self->x + self.self->y * self.self->y)(ARB_PRECISION_FAST);
  return size < bound.squared();
}

template <typename Vector>
std::optional<bool> detail::VectorWithError<Vector>::operator<=(Bound bound) const {
  const auto gt = *this > bound;

  if (!gt)
    return std::nullopt;

  return !*gt;
}

template <typename Vector>
std::optional<bool> detail::VectorWithError<Vector>::operator==(const Vector& rhs) const {
  const Vector& self = static_cast<const Vector&>(*this);

  auto maybe_x = self.self->x == rhs.self->x;
  if (!maybe_x || !*maybe_x)
    return maybe_x;

  auto maybe_y = self.self->x == rhs.self->y;
  if (!maybe_y || !*maybe_y)
    return maybe_y;

  return true;
}

template <typename Vector>
std::optional<bool> detail::VectorWithError<Vector>::operator!=(const Vector& rhs) const {
  const Vector& self = static_cast<const Vector&>(*this);

  const auto eq = self == rhs;

  if (eq) return !*eq;

  return std::nullopt;
}

template <typename Vector>
detail::VectorWithError<Vector>::operator std::optional<bool>() const {
  const Vector& self = static_cast<const Vector&>(*this);

  const auto x = self.self->x != exactreal::Arb(0);
  const auto y = self.self->y != exactreal::Arb(0);

  if (x && y)
    return *x || *y;
  if (x && *x)
    return true;
  if (y && *y)
    return true;
  return std::nullopt;
}

template <typename Vector>
exactreal::Arb detail::VectorWithError<Vector>::operator*(const Vector& rhs) const {
  const Vector& self = static_cast<const Vector&>(*this);

  return (self.self->x * rhs.self->x + self.self->y * rhs.self->y)(ARB_PRECISION_FAST);
}

template <typename Vector, typename T>
CCW detail::VectorExact<Vector, T>::ccw(const Vector& other) const {
  const Vector& self = static_cast<const Vector&>(*this);

  const auto ccwExact = [](const auto& lhs, const auto& rhs) {
    const T a = lhs.x() * rhs.y();
    const T b = rhs.x() * lhs.y();

    if (a == b) {
      return CCW::COLLINEAR;
    } else if (a > b) {
      return CCW::COUNTERCLOCKWISE;
    } else {
      return CCW::CLOCKWISE;
    }
  };

  if constexpr (IsEAntic<T> || IsExactReal<T>) {
    const auto maybeCcw = static_cast<flatsurf::Vector<exactreal::Arb>>(self).ccw(static_cast<flatsurf::Vector<exactreal::Arb>>(other));
    if (maybeCcw)
      return *maybeCcw;
  }

  return ccwExact(self, other);
}

template <typename Vector, typename T>
ORIENTATION detail::VectorExact<Vector, T>::orientation(const Vector& other) const {
  const Vector& self = static_cast<const Vector&>(*this);

  const auto orientationExact = [](const auto& lhs, const auto& rhs) {
    const T dot = lhs * rhs;

    if (!dot) {
      return ORIENTATION::ORTHOGONAL;
    } else if (dot > 0) {
      return ORIENTATION::SAME;
    } else {
      return ORIENTATION::OPPOSITE;
    }
  };

  if constexpr (IsEAntic<T> || IsExactReal<T>) {
    const auto maybeOrientation = static_cast<flatsurf::Vector<exactreal::Arb>>(self).orientation(static_cast<flatsurf::Vector<exactreal::Arb>>(other));
    if (maybeOrientation)
      return *maybeOrientation;
  }

  return orientationExact(self, other);
}

template <typename Vector, typename T>
T detail::VectorExact<Vector, T>::operator*(const Vector& rhs) const {
  const Vector& self = static_cast<const Vector&>(*this);

  return self.self->x * rhs.self->x + self.self->y * rhs.self->y;
}

template <typename Vector, typename T>
Vector& detail::VectorExact<Vector, T>::operator*=(const T& c) {
  Vector& self = static_cast<Vector&>(*this);

  self.self->x *= c;
  self.self->y *= c;

  return self;
}

template <typename Vector, typename T>
bool detail::VectorExact<Vector, T>::operator>(Bound bound) const {
  const Vector& self = static_cast<const Vector&>(*this);

  if (!bound) return static_cast<bool>(self);

  if constexpr (IsEAntic<T> || IsExactReal<T>) {
    const auto maybe = static_cast<flatsurf::Vector<exactreal::Arb>>(self) > bound;
    if (maybe)
      return *maybe;
  }

  return self.x() * self.x() + self.y() * self.y() > ::gmpxxll::mpz_class(bound.squared());
}

template <typename Vector, typename T>
bool detail::VectorExact<Vector, T>::operator<(Bound bound) const {
  const Vector& self = static_cast<const Vector&>(*this);

  if (!bound) return false;

  if constexpr (IsEAntic<T> || IsExactReal<T>) {
    const auto maybe = static_cast<flatsurf::Vector<exactreal::Arb>>(self) < bound;
    if (maybe)
      return *maybe;
  }

  return self.x() * self.x() + self.y() * self.y() < ::gmpxxll::mpz_class(bound.squared());
}

template <typename Vector, typename T>
bool detail::VectorExact<Vector, T>::operator==(const Vector& rhs) const {
  const Vector& self = static_cast<const Vector&>(*this);

  return self.x() == rhs.x() && self.y() == rhs.y();
}

template <typename Vector, typename T>
detail::VectorExact<Vector, T>::operator bool() const {
  const Vector& self = static_cast<const Vector&>(*this);

  return self.x() || self.y();
}

template <typename Vector, typename T>
bool detail::VectorExact<Vector, T>::inSector(const Vector& begin, const Vector& end) const {
  const Vector& self = static_cast<const Vector&>(*this);

  LIBFLATSURF_CHECK_ARGUMENT(begin, "zero vector cannot define a sector");
  LIBFLATSURF_CHECK_ARGUMENT(end, "zero vector cannot define a sector");
  LIBFLATSURF_CHECK_ARGUMENT(self, "zero vector containement in sector is not well defined");

  switch (begin.ccw(end)) {
    case CCW::COLLINEAR:
      switch (begin.orientation(end)) {
        case ORIENTATION::SAME:
          return ccw(begin) == CCW::COLLINEAR && orientation(begin) == ORIENTATION::SAME;
        case ORIENTATION::OPPOSITE:
          switch (begin.ccw(self)) {
            case CCW::COUNTERCLOCKWISE:
              return true;
            case CCW::CLOCKWISE:
              return false;
            case CCW::COLLINEAR:
              return begin.orientation(self) == ORIENTATION::SAME;
            default:
              LIBFLATSURF_UNREACHABLE("vectors must be counterclockwise, clockwise or collinear");
          }
        default:
          LIBFLATSURF_UNREACHABLE("non-zero collinear vectors cannot be orthogonal");
      }
    case CCW::CLOCKWISE:
      return !inSector(end, begin);
    case CCW::COUNTERCLOCKWISE:
      return begin.ccw(self) != CCW::CLOCKWISE && self.ccw(end) == CCW::COUNTERCLOCKWISE;
  }

  LIBFLATSURF_UNREACHABLE("two vectors must be clockwise, counter-clockwise, or collinear");
}

template <typename Vector, typename T>
T detail::VectorExact<Vector, T>::x() const {
  const Vector& self = static_cast<const Vector&>(*this);

  return self.self->x;
}

template <typename Vector, typename T>
T detail::VectorExact<Vector, T>::y() const {
  const Vector& self = static_cast<const Vector&>(*this);

  return self.self->y;
}

template <typename Vector, typename T>
T detail::VectorExact<Vector, T>::area(const std::vector<Vector>& perimeter) {
  Vector current;

  T area = T();

  for (const auto& v : perimeter) {
    Vector next = current + v;

    area += current.x() * next.y() - current.y() * next.x();

    current = next;
  }

  LIBFLATSURF_ASSERT(!current, fmt::format("Polygon must be closed but this polygon's sides [{}] summed to {}", fmt::join(perimeter, ", "), current));

  LIBFLATSURF_ASSERT(area >= 0, fmt::format("Area of polygon must be positive but the area of this polygon [{}] was {}; maybe the polygon was not oriented counterclockwise?", fmt::join(perimeter, ", "), area));

  return area;
}

template <typename Vector, typename T>
bool detail::VectorExact<Vector, T>::CompareSlope::operator()(const Vector& lhs, const Vector& rhs) const {
  LIBFLATSURF_ASSERT(lhs.x() || lhs.y(), "zero vector has no slope");
  LIBFLATSURF_ASSERT(rhs.x() || rhs.y(), "zero vector has no slope");

  const bool lhs_infinite = not lhs.x();
  const bool rhs_infinite = not rhs.x();

  if (rhs_infinite)
    return not lhs_infinite;
  else if (lhs_infinite)
    return false;
  else {
    const T a = lhs.y() * rhs.x();
    const T b = rhs.y() * lhs.x();

    if ((lhs.x() < 0) == (rhs.x() < 0))
      return a < b;
    else
      return a > b;
  }
}

template <typename Vector, typename T>
bool detail::VectorExact<Vector, T>::CompareLength::operator()(const Vector& lhs, const Vector& rhs) const {
  if (IsEAntic<T> || IsExactReal<T>) {
    const auto lhsArb = static_cast<flatsurf::Vector<exactreal::Arb>>(lhs);
    const auto rhsArb = static_cast<flatsurf::Vector<exactreal::Arb>>(rhs);

    const auto maybe = lhsArb * lhsArb < rhsArb * rhsArb;
    if (maybe)
      return *maybe;
  }

  return lhs * lhs < rhs * rhs;
}

template <typename T>
ImplementationOf<Vector<T>>::ImplementationOf() noexcept :
  x(),
  y() {}

template <typename T>
ImplementationOf<Vector<T>>::ImplementationOf(const T& x, const T& y) :
  x(x),
  y(y) {}

template <typename T>
ImplementationOf<Vector<T>>::ImplementationOf(T&& x, T&& y) :
  x(std::move(x)),
  y(std::move(y)) {}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& self) {
  return os << "(" << self.self->x << ", " << self.self->y << ")";
}

}  // namespace flatsurf

namespace std {

using namespace flatsurf;

template <typename T>
size_t hash<Vector<T>>::operator()(const Vector<T>& self) const {
  if constexpr (IsMPZ<T>) {
    return hash_combine(self.self->x.get_ui(), self.self->y.get_ui());
  } else if constexpr (IsMPQ<T>) {
    return hash_combine(self.self->x.get_num().get_ui(), self.self->x.get_den().get_ui(), self.self->y.get_num().get_ui(), self.self->y.get_den().get_ui());
  } else {
    return hash_combine(self.x(), self.y());
  }
}

}  // namespace std

// Instantiations of templates so implementations are generated for the linker
// (unfortunately, we also need to explicitly instantiate the base classes,
// https://stackoverflow.com/q/3705000/812379).
#include "util/instantiate.ipp"

#define LIBFLATSURF_INSTANTIATE_THIS(T)                    \
  LIBFLATSURF_INSTANTIATE_HASH((Vector<T>))                \
  LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION((Vector<T>)) \
  namespace flatsurf::detail {                             \
  template class VectorExact<Vector<T>, T>;                \
  template class VectorBase<Vector<T>>;                    \
  }

LIBFLATSURF_INSTANTIATE_MANY((LIBFLATSURF_INSTANTIATE_THIS), LIBFLATSURF_REAL_TYPES)

template class ::flatsurf::Vector<exactreal::Arb>;
template class ::flatsurf::detail::VectorWithError<::flatsurf::Vector<exactreal::Arb>>;
template class ::flatsurf::detail::VectorBase<::flatsurf::Vector<exactreal::Arb>>;
template std::ostream& ::flatsurf::operator<<(std::ostream&, const ::flatsurf::Vector<exactreal::Arb>&);
