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

#include <functional>
#include <ostream>

#include <boost/algorithm/string/join.hpp>
#include <boost/lexical_cast.hpp>

#include "../flatsurf/tracking.hpp"
#include "../flatsurf/tracking_set.hpp"

#include "impl/tracking_set.impl.hpp"

using std::ostream;
using std::string;

namespace flatsurf {

template <typename K>
TrackingSet<K>::TrackingSet(const FlatTriangulationCombinatorial* parent, const std::function<bool(const K&)>& values, const FlipHandler& updateAfterFlip, const CollapseHandler& updateBeforeCollapse) :
  impl(spimpl::make_unique_impl<Implementation>(this, parent, values, updateAfterFlip, updateBeforeCollapse)) {
}

template <typename K>
TrackingSet<K>::~TrackingSet() {}

template <typename K>
void TrackingSet<K>::insert(const K& key) {
  impl->set(key, true);
}

template <typename K>
void TrackingSet<K>::erase(const K& key) {
  impl->set(key, false);
}

template <typename K>
bool TrackingSet<K>::contains(const K& key) const {
  return impl->get(key);
}

template <typename K>
void TrackingSet<K>::apply(std::function<void(const K&)> map) const {
  for (const auto& k : Implementation::keys(parent()))
    if (contains(k))
      map(k);
}

template <typename K>
const FlatTriangulationCombinatorial& TrackingSet<K>::parent() const {
  return impl->tracker.parent();
}

template <typename K>
ostream& operator<<(ostream& os, const TrackingSet<K>& self) {
  std::vector<string> items;
  self.apply([&](const K& key) {
    items.push_back(boost::lexical_cast<string>(key));
  });
  return os << "{ " << boost::algorithm::join(items, ", ") << " }";
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "../flatsurf/edge.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/vertex.hpp"

namespace flatsurf {

template class TrackingSet<HalfEdge>;
template ostream& operator<<(ostream&, const TrackingSet<HalfEdge>&);
template class TrackingSet<Edge>;
template ostream& operator<<(ostream&, const TrackingSet<Edge>&);
template class TrackingSet<Vertex>;
template ostream& operator<<(ostream&, const TrackingSet<Vertex>&);

}  // namespace flatsurf
