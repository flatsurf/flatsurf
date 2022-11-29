/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2022 Julian Rüth
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

#include <string>

#include "../flatsurf/equivalence_class_code.hpp"

namespace flatsurf {

EquivalenceClassCode::~EquivalenceClassCode() {}

bool operator==(const EquivalenceClassCode& lhs, const EquivalenceClassCode& rhs) {
  return lhs.equal(rhs);
}

bool operator!=(const EquivalenceClassCode& lhs, const EquivalenceClassCode& rhs) {
  return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const EquivalenceClassCode& code) {
  return os << code.toString();
}

}

namespace std {

using namespace flatsurf;

size_t hash<EquivalenceClassCode>::operator()(const EquivalenceClassCode& self) const {
  return self.hash();
}

}
