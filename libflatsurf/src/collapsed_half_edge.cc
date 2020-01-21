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

#include "impl/collapsed_half_edge.hpp"

namespace flatsurf {

template <typename T>
void CollapsedHalfEdge<T>::updateAfterFlip(HalfEdgeMap<CollapsedHalfEdge>&, HalfEdge) {
  // intentionally empty, Implementation::updateAfterFlip() already did all the necessary updates.
}
 
template <typename T>
void CollapsedHalfEdge<T>::updateBeforeCollapse(HalfEdgeMap<CollapsedHalfEdge>&, Edge) {
  // intentionally empty, CollapsedHalfEdge::updateBeforeFlip already did all the necessary updates
}

template <typename T>
bool CollapsedHalfEdge<T>::operator==(const CollapsedHalfEdge& rhs) const {
  return connections == rhs.connections;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const CollapsedHalfEdge<T>& self) {
  std::vector<std::string> items;
  for (const auto& c : self.connections) {
    items.push_back(boost::lexical_cast<std::string>(c));
  }
  if (items.size()) {
    return os << "(" << boost::algorithm::join(items, "⇒") << ")";
  } else {
    return os;
  }
}

}

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), CollapsedHalfEdge, LIBFLATSURF_REAL_TYPES)
