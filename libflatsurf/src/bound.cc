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

#include <ostream>

#include "../flatsurf/bound.hpp"

namespace flatsurf {

Bound::Bound(const mpz_class& x, const mpz_class& y) :
  square(x * x + y * y) {}

const mpz_class& Bound::squared() const noexcept {
  return square;
}

bool Bound::operator==(const Bound& rhs) const noexcept {
  return square == rhs.square;
}

std::ostream& operator<<(std::ostream& os, const Bound& self) {
  return os << "√(" << self.squared() << ")";
}

}  // namespace flatsurf
