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

#include "../../flatsurf/vector.hpp"

#include <boost/lexical_cast.hpp>
#include <exact-real/arb.hpp>
#include <exact-real/arf.hpp>
#include <exact-real/element.hpp>
#include <exact-real/integer_ring.hpp>
#include <exact-real/number_field.hpp>
#include <exact-real/rational_field.hpp>
#include <exact-real/yap/arb.hpp>

#include "../external/gmpxxll/gmpxxll/mpz_class.hpp"
#include "../util/assert.ipp"
#include "../util/hash.ipp"
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
using exactreal::Arf;

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
class ImplementationOf<Vector<T>> : public Cartesian<T> {
 public:
  using Cartesian<T>::Cartesian;
  using Vector = flatsurf::Vector<T>;

  static Vector make(const T& x, const T& y) {
    return Vector(x, y);
  }

  template <bool Enable = IsArb<T>, If<Enable> = true>
  ImplementationOf& operator+=(const flatsurf::Vector<Arb>& rhs) {
    // Unfortunately, exact-real's arb::yap adds too much overhead, in
    // particular in DEBUG builds for this operation that is called all the
    // time. So we have to call arb_add directly here.

    // this->x += rhs.self->x(ARB_PRECISION_FAST);
    arb_add(this->x.arb_t(), this->x.arb_t(), rhs.self->x.arb_t(), ARB_PRECISION_FAST);

    // this->y += rhs.self->y(ARB_PRECISION_FAST);
    arb_add(this->y.arb_t(), this->y.arb_t(), rhs.self->y.arb_t(), ARB_PRECISION_FAST);

    return *this;
  }

  template <typename S, bool Enable = IsArb<T>, If<Enable> = true>
  ImplementationOf& operator*=(const S& rhs) {
    this->x *= Arb(rhs)(ARB_PRECISION_FAST);
    this->y *= Arb(rhs)(ARB_PRECISION_FAST);
    return *this;
  }

  template <typename S, bool Enable = IsLongLong<T>&& IsMPZ<S>, If<Enable> = true, typename = void>
  ImplementationOf& operator*=(const S& rhs) {
    using gmpxxll::mpz_class;
    ASSERT(rhs * mpz_class(this->x) <= mpz_class(LONG_LONG_MAX), "Multiplication overflow");
    ASSERT(rhs * mpz_class(this->y) <= mpz_class(LONG_LONG_MAX), "Multiplication overflow");
    this->x *= mpz_class(rhs).get_sll();
    this->y *= mpz_class(rhs).get_sll();
    return *this;
  }

  template <typename S, bool Enable = IsArb<T>, If<Enable> = true>
  ImplementationOf& operator/=(const S& rhs) {
    this->x /= Arb(rhs)(ARB_PRECISION_FAST);
    this->y /= Arb(rhs)(ARB_PRECISION_FAST);
    return *this;
  }

  template <typename S, bool Enable = IsLongLong<T>&& IsMPZ<S>, If<Enable> = true, typename = void>
  ImplementationOf& operator/=(const S& rhs) {
    using gmpxxll::mpz_class;
    if (rhs >= mpz_class(LONG_LONG_MAX)) {
      this->x = 0;
      this->y = 0;
    }
    this->x /= mpz_class(rhs).get_sll();
    this->y /= mpz_class(rhs).get_sll();
    return *this;
  }

  template <typename S, bool Enable = Similar<T, exactreal::Element<exactreal::IntegerRing>>, If<Enable> = true, typename = void, typename = void>
  ImplementationOf& operator/=(const S&) {
    throw std::logic_error("not implemented: division of Element<IntegerRing>");
  }

  template <bool Enable = IsMPZ<T>, If<Enable> = true>
  size_t hash() const {
    return hash_combine(this->x.get_ui(), this->y.get_ui());
  }

  template <bool Enable = IsMPQ<T>, If<Enable> = true, typename = void>
  size_t hash() const {
    return hash_combine(this->x.get_num().get_ui(), this->x.get_den().get_ui(), this->y.get_num().get_ui(), this->y.get_den().get_ui());
  }

  template <bool Enable = IsEAntic<T>, If<Enable> = true, typename = void, typename = void>
  size_t hash() const {
    size_t ret = hash_combine(this->x.den().get_ui(), this->y.den().get_ui());
    for (const auto& c : this->x.num_vector())
      ret = hash_combine(ret, c.get_ui());
    for (const auto& c : this->y.num_vector())
      ret = hash_combine(ret, c.get_ui());
    return ret;
  }

  // Delete this method once exactreal::Element is hashable, https://github.com/flatsurf/flatsurf/issues/154.
  template <bool Enable = IsExactReal<T>, If<Enable> = true, typename = void, typename = void, typename = void>
  size_t hash() const {
    return hash_combine(this->x.floor().get_ui(), this->y.floor().get_ui());
  }

  template <bool Enable = IsArb<T>, If<Enable> = true>
  std::optional<CCW> ccw(const flatsurf::Vector<exactreal::Arb>& rhs) const {
    const Arb a = (this->x * rhs.self->y)(ARB_PRECISION_FAST);
    const Arb b = (rhs.self->x * this->y)(ARB_PRECISION_FAST);

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

  template <bool Enable = IsArb<T>, If<Enable> = true>
  std::optional<ORIENTATION> orientation(const flatsurf::Vector<exactreal::Arb>& rhs) const {
    // Arb also has a built-in dot product. It's probably not doing anything else in 2d.
    const Arb dot = (this->x * rhs.self->x + this->y * rhs.self->y)(ARB_PRECISION_FAST);

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
    return std::nullopt;
  }

  template <bool Enable = IsArb<T>, If<Enable> = true>
  std::optional<bool> operator<(const Bound bound) const {
    if (!bound) return false;
    Arb size = (this->x * this->x + this->y * this->y)(ARB_PRECISION_FAST);
    return size < bound.squared();
  }

  template <bool Enable = IsArb<T>, If<Enable> = true>
  std::optional<bool> operator>(const Bound bound) const {
    if (!bound) {
      auto nonzero = static_cast<std::optional<bool>>(*this);
      if (nonzero) return *nonzero;
    }
    Arb size = (this->x * this->x + this->y * this->y)(ARB_PRECISION_FAST);
    return size > bound.squared();
  }

  template <bool Enable = IsArb<T>, If<Enable> = true>
  operator std::optional<bool>() const {
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
    return make((dot * rhs.self->x)(ARB_PRECISION_FAST),
        (dot * rhs.self->y)(ARB_PRECISION_FAST));
  }

  template <bool Enable = IsArb<T>, If<Enable> = true>
  Arb operator*(const Vector& rhs) const {
    return (this->x * rhs.self->x + this->y * rhs.self->y)(ARB_PRECISION_FAST);
  }
};

template <typename T>
Vector<T>::Vector() noexcept :
  self(spimpl::make_impl<ImplementationOf<Vector>>(T(), T())) {}

template <typename T>
Vector<T>::Vector(const T& x, const T& y) :
  self(spimpl::make_impl<ImplementationOf<Vector>>(x, y)) {}

template <typename T>
T Vector<T>::x() const { return self->x; }

template <typename T>
T Vector<T>::y() const { return self->y; }

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& self) {
  using Implementation = ImplementationOf<Vector<T>>;
  const Vector<T>& s = static_cast<const Vector<T>&>(self);

  if constexpr (has_ostream_lshift<Implementation>) {
    return os << s.self;
  } else if constexpr (is_forward_v<Implementation>) {
    return os << s.self->vector;
  } else if constexpr (has_approximation_v<Implementation>) {
    return os << s.self->approximation();
  } else if constexpr (is_cartesian_v<Implementation>) {
    return os << "(" << s.self->x << ", " << s.self->y << ")";
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing operator<<.");
  }
}
}  // namespace flatsurf

namespace std {

using namespace flatsurf;

template <typename T>
size_t hash<Vector<T>>::operator()(const Vector<T>& self) const {
  using Implementation = ImplementationOf<Vector<T>>;
  const Vector<T>& s = static_cast<const Vector<T>&>(self);

  if constexpr (has_hash<Implementation>) {
    return s.self->hash();
  } else if constexpr (is_forward_v<Implementation>) {
    return std::hash<decltype(s.self->vector)>{}(s.self->vector);
  } else if constexpr (is_cartesian_v<Implementation>) {
    return hash_combine(s.x(), s.y());
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing hash().");
  }
}

}  // namespace std

// Instantiations of templates so implementations are generated for the linker
// (unfortunately, we also need to explicitly instantiate the base classes,
// https://stackoverflow.com/q/3705000/812379).
#include "../util/instantiate.ipp"

#define LIBFLATSURF_INSTANTIATE_THIS(T)                       \
  LIBFLATSURF_INSTANTIATE_HASH((Vector<T>))                   \
  LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION((Vector<T>)) \
  namespace flatsurf::detail {                                \
  template class VectorExact<Vector<T>, T>;                   \
  template class VectorBase<Vector<T>>;                       \
  }

LIBFLATSURF_INSTANTIATE_MANY((LIBFLATSURF_INSTANTIATE_THIS), LIBFLATSURF_REAL_TYPES)

template class ::flatsurf::Vector<Arb>;
template class ::flatsurf::detail::VectorWithError<::flatsurf::Vector<Arb>>;
template class ::flatsurf::detail::VectorBase<::flatsurf::Vector<Arb>>;
template std::ostream& ::flatsurf::operator<<(std::ostream&, const ::flatsurf::Vector<Arb>&);
