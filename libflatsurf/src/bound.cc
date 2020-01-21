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

#include "../flatsurf/bound.hpp"

namespace flatsurf {

Bound::Bound(long x, long y) :
  square(0) {
    long long a=0, b=0;
  {
    bool overflow = __builtin_smulll_overflow(x, x, &a);
    assert(!overflow && "long long overflow in Bound computation");
  }
  {
    bool overflow = __builtin_smulll_overflow(y, y, &b);
    assert(!overflow && "long long overflow in Bound computation");
  }
  {
    bool overflow = __builtin_saddll_overflow(a, b, &square);
    assert(!overflow && "long long overflow in Bound computation");
  }
}

long long Bound::squared() const noexcept {
  return square;
}

bool Bound::operator==(const Bound& rhs) const noexcept {
  return square == rhs.square;
}

}
