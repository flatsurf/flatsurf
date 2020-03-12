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

#include "../flatsurf/edge.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/permutation.hpp"

#include "util/assert.ipp"

using std::ostream;

namespace flatsurf {

HalfEdge::HalfEdge(PrivateConstructor, size_t idx) :
  idx(idx) {
}

HalfEdge::HalfEdge(const int id) :
  HalfEdge(PrivateConstructor{}, id > 0 ? (2 * (id - 1)) : (-2 * id - 1)) {
  ASSERT_ARGUMENT(id != 0, "id must be non-zero");
}

HalfEdge HalfEdge::fromIndex(const size_t index) {
  return HalfEdge(PrivateConstructor{}, index);
}

HalfEdge::HalfEdge() :
  HalfEdge(PrivateConstructor{}, static_cast<size_t>(-1)) {}

int HalfEdge::id() const noexcept {
  ASSERT(idx != static_cast<size_t>(-1), "id must be non-zero");
  return (idx % 2 ? -1 : 1) * static_cast<int>(idx / 2 + 1);
}

HalfEdge HalfEdge::operator-() const {
  return HalfEdge::fromIndex(idx ^ static_cast<size_t>(1));
}

HalfEdge &HalfEdge::operator=(const HalfEdge &rhs) {
  idx = rhs.idx;
  ASSERT(idx != static_cast<size_t>(-1), "id must be non-zero");
  return *this;
}

bool HalfEdge::operator==(const HalfEdge &rhs) const {
  return idx == rhs.idx;
}

size_t HalfEdge::index() const noexcept {
  ASSERT(idx != static_cast<size_t>(-1), "id must be non-zero");
  return idx;
}

Edge HalfEdge::edge() const {
  return Edge(*this);
}

ostream &operator<<(ostream &os, const HalfEdge &self) { return os << self.id(); }

}  // namespace flatsurf

size_t std::hash<flatsurf::HalfEdge>::operator()(const flatsurf::HalfEdge &e) const noexcept {
  return static_cast<size_t>(e.index());
}
