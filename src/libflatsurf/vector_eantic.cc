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

#include <boost/numeric/conversion/cast.hpp>
#include <exact-real/arb.hpp>
#include <ostream>
#include "external/spimpl/spimpl.h"

#include "flatsurf/bound.hpp"
#include "flatsurf/vector_arb.hpp"
#include "flatsurf/vector_eantic.hpp"

using boost::numeric_cast;
using eantic::renf_elem_class;
using exactreal::Arb;
using std::ostream;

namespace flatsurf {
struct VectorEAntic::Implementation {
  Implementation(const renf_elem_class &x, const renf_elem_class &y,
                 const mp_limb_signed_t precision)
      : x(x), y(y), precision(precision) {}

  void refresh() {
    approximation = VectorArb(Arb(x, precision), Arb(y, precision));
  }

  renf_elem_class x;
  renf_elem_class y;
  const mp_limb_signed_t precision;
  VectorArb approximation;
};

VectorEAntic::VectorEAntic()
    : VectorEAntic(renf_elem_class(), renf_elem_class()) {}

VectorEAntic::VectorEAntic(const renf_elem_class &x, const renf_elem_class &y)
    // TODO: What is the correct precision here?
    : impl(spimpl::make_impl<Implementation>(x, y, VectorArb::prec)) {
  impl->refresh();
}

VectorEAntic VectorEAntic::operator-() const {
  return VectorEAntic(-impl->x, -impl->y);
}

VectorEAntic &VectorEAntic::operator+=(const VectorEAntic &rhs) {
  impl->x += rhs.impl->x;
  impl->y += rhs.impl->y;
  impl->refresh();
  return *this;
}

VectorEAntic &VectorEAntic::operator-=(const VectorEAntic &rhs) {
  impl->x -= rhs.impl->x;
  impl->y -= rhs.impl->y;
  return *this;
}

VectorEAntic::operator const VectorArb &() const { return impl->approximation; }

VectorEAntic::operator bool() const { return impl->x != 0 || impl->y != 0; }

CCW VectorEAntic::ccw(const VectorEAntic &other) const {
  const auto a = impl->x * other.impl->y;
  const auto b = impl->y * other.impl->x;

  if (a > b) {
    return CCW::COUNTERCLOCKWISE;
  } else if (a < b) {
    return CCW::CLOCKWISE;
  } else {
    return CCW::COLLINEAR;
  }
}

ORIENTATION VectorEAntic::orientation(const VectorEAntic &rhs) const {
  auto dot = impl->x * rhs.impl->x + impl->y * rhs.impl->y;
  if (dot > 0) {
    return ORIENTATION::SAME;
  } else if (dot == 0) {
    return ORIENTATION::ORTHOGONAL;
  } else {
    return ORIENTATION::OPPOSITE;
  }
}

bool VectorEAntic::operator<(const Bound bound) const {
  // Strangely, e-antic does not have operators for long long.
  return impl->x * impl->x + impl->y * impl->y <
         numeric_cast<unsigned long>(bound.squared);
}

bool VectorEAntic::operator>(const Bound bound) const {
  // Strangely, e-antic does not have operators for long long.
  return impl->x * impl->x + impl->y * impl->y >
         numeric_cast<unsigned long>(bound.squared);
}

bool VectorEAntic::operator==(const VectorEAntic &rhs) const {
  return impl->x == rhs.impl->x && impl->y == rhs.impl->y;
}

VectorEAntic &VectorEAntic::operator*=(const int rhs) {
  impl->x *= rhs;
  impl->y *= rhs;
  impl->refresh();
  return *this;
}

ostream &operator<<(ostream &os, const VectorEAntic &self) {
  return os << "(" << self.impl->x << "," << self.impl->y << ")";
}
}  // namespace flatsurf
