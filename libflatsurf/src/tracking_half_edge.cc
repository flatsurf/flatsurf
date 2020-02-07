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

#include "../flatsurf/tracking_half_edge.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/flat_triangulation_combinatorial.hpp"
#include "../flatsurf/half_edge.hpp"

#include "impl/tracking_half_edge.impl.hpp"

namespace flatsurf {

TrackingHalfEdge::TrackingHalfEdge(const FlatTriangulationCombinatorial* parent, HalfEdge value, bool followFlip) :
  impl(spimpl::make_unique_impl<Implementation>(parent, value, followFlip)) {
}

TrackingHalfEdge::~TrackingHalfEdge() {}

HalfEdge TrackingHalfEdge::operator-() const {
  return -static_cast<HalfEdge>(*this);
}

TrackingHalfEdge::operator HalfEdge() const {
  HalfEdge ret;
  impl->value.apply([&](HalfEdge e) {
    ret = e;
  });
  return ret;
}

TrackingHalfEdge& TrackingHalfEdge::operator=(HalfEdge e) {
  impl->value.erase(static_cast<HalfEdge>(*this));
  impl->value.insert(e);
  return *this;
}

TrackingHalfEdge::Implementation::Implementation(const FlatTriangulationCombinatorial* parent, HalfEdge value, bool followFlip) :
  value(
      parent, [&](HalfEdge e) { return e == value; }, followFlip ? Implementation::followFlip : Implementation::noFlip, Implementation::noCollapse) {
}

void TrackingHalfEdge::Implementation::followFlip(HalfEdgeSet&, HalfEdge) {  // intentionally empty
}

void TrackingHalfEdge::Implementation::noFlip(HalfEdgeSet& value, HalfEdge flip) {
  if (value.contains(flip) || value.contains(-flip))
    throw std::logic_error("this TrackingHalfEdge cannot be flipped");
}

void TrackingHalfEdge::Implementation::noCollapse(HalfEdgeSet& value, Edge collapse) {
  const auto& parent = value.parent();
  const HalfEdge e = collapse.positive();
  if (value.contains(e) || value.contains(parent.nextInFace(e)) || value.contains(parent.previousInFace(e)) || value.contains(-e) || value.contains(parent.nextInFace(-e)) || value.contains(parent.previousInFace(-e)))
    throw std::logic_error("a TrackingHalfEdge cannot be collapsed");
}

}  // namespace flatsurf
