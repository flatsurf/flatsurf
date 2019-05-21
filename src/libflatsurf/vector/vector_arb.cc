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

#include <exact-real/yap/arb.hpp>
#include <ostream>

#include "flatsurf/bound.hpp"
#include "flatsurf/vector_arb.hpp"
#include "flatsurf/vector_eantic.hpp"

using exactreal::Arb;
using std::optional;
using std::ostream;
// Currently, we use this precision for all computations. This is not really a
// good choice, see https://github.com/flatsurf/flatsurf/issues/52.
using exactreal::ARB_PRECISION_FAST;

namespace flatsurf {
VectorArb::VectorArb() {}

VectorArb::VectorArb(const Arb &x, const Arb &y) : x(x), y(y) {}

bool VectorArb::isExact() const { return x.is_exact() && y.is_exact(); }

VectorArb &VectorArb::operator+=(const VectorArb &rhs) {
  x += (rhs.x)(ARB_PRECISION_FAST);
  y += (rhs.y)(ARB_PRECISION_FAST);
  return *this;
}

VectorArb &VectorArb::operator*=(const int rhs) {
  arb_mul_si(x.arb_t(), x.arb_t(), rhs, ARB_PRECISION_FAST);
  arb_mul_si(y.arb_t(), y.arb_t(), rhs, ARB_PRECISION_FAST);
  return *this;
}

optional<bool> VectorArb::operator<(const Bound bound) const {
  Arb size = (x * x + y * y)(ARB_PRECISION_FAST);
  return size < bound.squared;
}

optional<bool> VectorArb::operator>(const Bound bound) const {
  Arb size = (x * x + y * y)(ARB_PRECISION_FAST);
  return size > bound.squared;
}

optional<bool> VectorArb::operator==(const VectorArb &rhs) const {
  auto xeq = x == rhs.x;
  auto yeq = y == rhs.y;

  if (xeq && *xeq && yeq && *yeq) {
    return true;
  }
  if ((xeq && !*xeq) || (yeq && !*yeq)) {
    return false;
  }

  return {};
}

optional<bool> VectorArb::operator!=(const VectorArb &rhs) const {
  auto eq = *this == rhs;
  return eq ? !*eq : eq;
}

VectorArb::operator std::complex<double>() const {
  return std::complex(static_cast<double>(x), static_cast<double>(y));
}

optional<CCW> VectorArb::ccw(const VectorArb &rhs) const {
  const Arb a = (x * rhs.y)(ARB_PRECISION_FAST);
  const Arb b = (rhs.x * y)(ARB_PRECISION_FAST);

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

optional<ORIENTATION> VectorArb::orientation(const VectorArb &rhs) const {
  // Arb also has a built-in dot product. It's probably not doing anything else
  // in 2d.
  const Arb dot = (x * rhs.x + y * rhs.y)(ARB_PRECISION_FAST);

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

ostream &operator<<(ostream &os, const VectorArb &self) {
  return os << "(" << self.x << ", " << self.y << ")";
}

}  // namespace flatsurf
