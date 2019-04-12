/**********************************************************************
 *  This file is part of Flatsurf.
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
 *  along with Flatsurf. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#include <exact-real/yap/arb.hpp>
#include <ostream>

#include "libflatsurf/bound.hpp"
#include "libflatsurf/vector_arb.hpp"
#include "libflatsurf/vector_eantic.hpp"

using exactreal::Arb;
using std::optional;
using std::ostream;

namespace flatsurf {
VectorArb::VectorArb() {}

VectorArb::VectorArb(const Arb& x, const Arb& y) : x(x), y(y) {}

bool VectorArb::isExact() const { return x.is_exact() && y.is_exact(); }

VectorArb& VectorArb::operator+=(const VectorArb& rhs) {
  // TODO: Should we really fix a precision here?
  x += (rhs.x)(ARF_PREC_EXACT);
  y += (rhs.y)(ARF_PREC_EXACT);
  return *this;
}

VectorArb& VectorArb::operator*=(const int rhs) {
  // TODO: Should we really fix a precision here?
  arb_mul_si(x.arb_t(), x.arb_t(), rhs, ARF_PREC_EXACT);
  arb_mul_si(y.arb_t(), y.arb_t(), rhs, ARF_PREC_EXACT);
  return *this;
}

optional<bool> VectorArb::operator<(const Bound bound) const {
  // TODO: Should we really fix a precision here?
  Arb size = (x * x + y * y)(ARF_PREC_EXACT);
  return size < bound.squared;
}

optional<bool> VectorArb::operator>(const Bound bound) const {
  // TODO: Should we really fix a precision here?
  Arb size = (x * x + y * y)(ARF_PREC_EXACT);
  return size > bound.squared;
}

optional<CCW> VectorArb::ccw(const VectorArb& rhs) const {
  // TODO: Should we really fix a precision here? No.
  const Arb a = (x * rhs.y)(ARF_PREC_EXACT);
  const Arb b = (rhs.x * y)(ARF_PREC_EXACT);

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

optional<ORIENTATION> VectorArb::orientation(const VectorArb& rhs) const {
  // TODO: Should we really fix a precision here?
  // TODO: Arb also has a dot product. In such a low dimension it probably does
  // not make a difference.
  const Arb dot = (x * rhs.x + y * rhs.y)(ARF_PREC_EXACT);

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

ostream& operator<<(ostream& os, const VectorArb& self) {
  return os << "(" << self.x << ", " << self.y << ")";
}

mp_limb_signed_t VectorArb::prec = 64;
}  // namespace flatsurf
