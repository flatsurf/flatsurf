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

#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/permutation.hpp"

#include "util/assert.ipp"

using std::ostream;

namespace flatsurf {
HalfEdge::HalfEdge(const int id) :
  id(id) {
  ASSERT_ARGUMENT(id != 0, "id must be non-zero");
}

HalfEdge::HalfEdge() :
  id(0) {}

HalfEdge HalfEdge::operator-() const {
  ASSERT_ARGUMENT(id != 0, "id must be non-zero");
  return HalfEdge(-id);
}

HalfEdge &HalfEdge::operator=(const HalfEdge &rhs) {
  this->id = rhs.id;
  ASSERT_ARGUMENT(id != 0, "id must be non-zero");
  return *this;
}

bool HalfEdge::operator==(const HalfEdge &rhs) const {
  return this->id == rhs.id;
}

bool HalfEdge::operator<(const HalfEdge &rhs) const {
  return index() < rhs.index();
}

size_t HalfEdge::index() const noexcept {
  const int id = this->id;
  ASSERT(id != 0, "a valid half edge must have a non-zero id");
  if (id < 0) {
    return static_cast<size_t>(-2 * id - 1);
  } else {
    return static_cast<size_t>(2 * id - 2);
  }
}

ostream &operator<<(ostream &os, const HalfEdge &self) { return os << self.id; }

}  // namespace flatsurf

size_t std::hash<flatsurf::HalfEdge>::operator()(const flatsurf::HalfEdge &e) const noexcept {
  return static_cast<size_t>(e.id);
}
