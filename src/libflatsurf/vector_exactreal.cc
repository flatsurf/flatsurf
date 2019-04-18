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

#include <e-antic/renfxx.h>
#include <boost/numeric/conversion/cast.hpp>
#include <exact-real/arb.hpp>
#include <exact-real/element.hpp>
#include <ostream>
#include "external/spimpl/spimpl.h"

#include "flatsurf/bound.hpp"
#include "flatsurf/vector_arb.hpp"
#include "flatsurf/vector_exactreal.hpp"

using boost::numeric_cast;
using exactreal::Arb;
using exactreal::Element;
using std::ostream;

namespace flatsurf {
template <typename Ring>
struct VectorExactReal<Ring>::Implementation {
  Implementation(const Element<Ring>& x, const Element<Ring>& y,
                 // TODO: Is this a good precision?
                 const mp_limb_signed_t precision = VectorArb::prec)
      : x(x), y(y), precision(precision) {}

  Implementation(const Implementation& rhs)
      : x(rhs.x),
        y(rhs.y),
        precision(rhs.precision),
        approximation(rhs.approximation) {}

  Implementation(const Implementation&& rhs) = delete;

  Implementation& operator=(const Implementation&) = delete;
  Implementation& operator=(const Implementation&&) = delete;

  void refresh() {
    approximation = VectorArb(x.arb(precision), y.arb(precision));
  }

  Element<Ring> x;
  Element<Ring> y;
  const mp_limb_signed_t precision;
  VectorArb approximation;
};

template <typename Ring>
VectorExactReal<Ring>::VectorExactReal()
    : impl(
          spimpl::make_impl<Implementation>(Element<Ring>(), Element<Ring>())) {
  impl->refresh();
}

template <typename Ring>
VectorExactReal<Ring>::VectorExactReal(const Element<Ring>& x,
                                       const Element<Ring>& y)
    : impl(spimpl::make_impl<Implementation>(x, y)) {
  impl->refresh();
}

template <typename Ring>
VectorExactReal<Ring> VectorExactReal<Ring>::operator-() const {
  return VectorExactReal<Ring>(-impl->x, -impl->y);
}

template <typename Ring>
VectorExactReal<Ring>& VectorExactReal<Ring>::operator+=(
    const VectorExactReal<Ring>& rhs) {
  impl->x += rhs.impl->x;
  impl->y += rhs.impl->y;
  impl->approximation += rhs.impl->approximation;
  return *this;
}

template <typename Ring>
VectorExactReal<Ring>& VectorExactReal<Ring>::operator-=(
    const VectorExactReal<Ring>& rhs) {
  impl->x -= rhs.impl->x;
  impl->y -= rhs.impl->y;
  return *this;
}

template <typename Ring>
VectorExactReal<Ring>::operator const VectorArb&() const {
  return impl->approximation;
}

template <typename Ring>
VectorExactReal<Ring>::operator bool() const {
  return static_cast<bool>(impl->x) || static_cast<bool>(impl->y);
}

template <typename Ring>
CCW VectorExactReal<Ring>::ccw(const VectorExactReal<Ring>& other) const {
  // The only tricky part is to find out whether x*other.y == y*other.x.
  // Since all four are elements of the same submodule of R, this can only
  // happen, if already in the field of scalars: x/other.x == y/other.y or
  // similarly for any of the other quotients.
  if (!impl->x || !impl->y || !other.impl->x || !other.impl->y) {
    if ((!impl->x || !other.impl->y) && (!impl->y || !other.impl->x)) {
      return CCW::COLLINEAR;
    }
  } else {
    const auto xquot = impl->x / other.impl->x;
    if (xquot.has_value()) {
      const auto yquot = impl->y / other.impl->y;
      if (yquot.has_value()) {
        if (*xquot == *yquot) {
          return CCW::COLLINEAR;
        }
      }
    }

    const auto selfquot = impl->x / impl->y;
    if (selfquot.has_value()) {
      const auto otherquot = other.impl->x / other.impl->y;
      if (otherquot.has_value()) {
        if (*selfquot == *otherquot) {
          return CCW::COLLINEAR;
        }
      }
    }
  }

  // Now we know that the two vectors cannot be collinear. At some
  // precision we must be able to find out what is their orientation.
  for (long prec = 64;; prec *= 2) {
    const auto self = VectorArb(impl->x.arb(prec), impl->y.arb(prec));
    const auto rhs =
        VectorArb(other.impl->x.arb(prec), other.impl->y.arb(prec));
    const auto ret = self.ccw(rhs);
    if (ret.has_value()) {
      return *ret;
    }
  }
}

template <typename Ring>
ORIENTATION VectorExactReal<Ring>::orientation(
    const VectorExactReal<Ring>& other) const {
  // We need to compute the dot product of this vector and other to decide their
  // orientation. The same discussion as for ccw also applies here, i.e.,
  // x*other.x == -y*other.y can only happen if there is already a relation in
  // the field of scalars: x/y == -other.x/other.y or similar.
  if (!impl->x || !impl->y || !other.impl->x || !other.impl->y) {
    if ((!impl->x || !other.impl->x) && (!impl->y || !other.impl->y)) {
      return ORIENTATION::ORTHOGONAL;
    }
  } else {
    const auto quot = impl->x / other.impl->y;
    if (quot.has_value()) {
      const auto otherquot = impl->y / other.impl->x;
      if (otherquot.has_value()) {
        if (*quot == -*otherquot) {
          return ORIENTATION::ORTHOGONAL;
        }
      }
    }

    const auto selfquot = impl->x / impl->y;
    if (selfquot.has_value()) {
      const auto otherquot = other.impl->y / other.impl->x;
      if (otherquot.has_value()) {
        if (*selfquot == -*otherquot) {
          return ORIENTATION::ORTHOGONAL;
        }
      }
    }
  }

  // Now we know that the two vectors cannot be orthogonal. At some precision
  // their orientation is going to show.
  for (long prec = 64;; prec *= 2) {
    const auto self = VectorArb(impl->x.arb(prec), impl->y.arb(prec));
    const auto rhs =
        VectorArb(other.impl->x.arb(prec), other.impl->y.arb(prec));
    const auto ret = self.orientation(other);
    if (ret.has_value()) {
      return *ret;
    }
  }
}

template <typename Ring>
bool VectorExactReal<Ring>::operator<(const Bound bound) const {
  // TODO: Test that an integer element can decide that it's not smaller than
  // its exact value.
  for (long prec = 64;; prec *= 2) {
    const auto v = VectorArb(impl->x.arb(prec), impl->y.arb(prec));
    const auto lt = v < bound;
    if (lt.has_value()) {
      return *lt;
    }
  }
}

template <typename Ring>
bool VectorExactReal<Ring>::operator>(const Bound bound) const {
  // TODO: Test that an integer element can decide that it's not greater than
  // its exact value.
  for (long prec = 64;; prec *= 2) {
    const auto v = VectorArb(impl->x.arb(prec), impl->y.arb(prec));
    const auto gt = v > bound;
    if (gt.has_value()) {
      return *gt;
    }
  }
}

template <typename Ring>
bool VectorExactReal<Ring>::operator==(const VectorExactReal<Ring>& rhs) const {
  return impl->x == rhs.impl->x && impl->y == rhs.impl->y;
}

template <typename Ring>
VectorExactReal<Ring>& VectorExactReal<Ring>::operator*=(const int rhs) {
  impl->x *= rhs;
  impl->y *= rhs;
  impl->approximation *= rhs;
  return *this;
}

template <typename Ring>
ostream& operator<<(ostream& os, const VectorExactReal<Ring>& self) {
  return os << "(" << self.impl->x << "," << self.impl->y << ")";
}
}  // namespace flatsurf

using namespace flatsurf;

// Instantiations of templates so implementations are generated for the linker
#include <exact-real/number_field_traits.hpp>
template struct flatsurf::VectorExactReal<exactreal::NumberFieldTraits>;
template ostream& flatsurf::operator<<<exactreal::NumberFieldTraits>(
    ostream&, const VectorExactReal<exactreal::NumberFieldTraits>&);
