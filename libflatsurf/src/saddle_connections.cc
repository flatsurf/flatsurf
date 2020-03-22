/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2020 Julian Rüth
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

#include <exact-real/arb.hpp>
#include <stack>
#include <fmt/format.h>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/ccw.hpp"
#include "../flatsurf/chain.hpp"
#include "../flatsurf/fmt.hpp"
#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/saddle_connections_iterator.hpp"
#include "../flatsurf/vector.hpp"

#include "external/rx-ranges/include/rx/ranges.hpp"

#include "impl/saddle_connections.impl.hpp"
#include "impl/saddle_connections_iterator.impl.hpp"

#include "util/assert.ipp"

namespace flatsurf {

using std::vector;

template <typename Surface>
ImplementationOf<SaddleConnections<Surface>>::ImplementationOf(std::shared_ptr<const Surface> surface, Bound bound, const std::vector<HalfEdge>& sectors) :
  surface(surface),
  bound(bound),
  sectors(sectors) {}

template <typename Surface>
SaddleConnections<Surface>::SaddleConnections(const std::shared_ptr<const Surface>& surface, const Bound searchRadius) :
  impl(spimpl::make_impl<Implementation>(surface, searchRadius, surface->halfEdges())) {}

template <typename Surface>
SaddleConnections<Surface>::SaddleConnections(const std::shared_ptr<const Surface>& surface, const Bound searchRadius, const Vertex source) :
  impl(spimpl::make_impl<Implementation>(surface, searchRadius, surface->outgoing(source))) {}

template <typename Surface>
SaddleConnections<Surface>::SaddleConnections(const std::shared_ptr<const Surface>& surface, const Bound searchRadius, const HalfEdge sectorBegin) :
  impl(spimpl::make_impl<Implementation>(surface, searchRadius, vector<HalfEdge>{sectorBegin})) {}

template <typename Surface>
typename SaddleConnections<Surface>::Iterator SaddleConnections<Surface>::begin() const {
  return Iterator(PrivateConstructor{}, impl->surface, impl->bound, std::begin(impl->sectors), std::end(impl->sectors));
}

template <typename Surface>
typename SaddleConnections<Surface>::Iterator SaddleConnections<Surface>::end() const {
  return Iterator(PrivateConstructor{}, impl->surface, impl->bound, std::end(impl->sectors), std::end(impl->sectors));
}

template <typename Surface>
template <typename ...Args>
SaddleConnectionsIterator<Surface>::SaddleConnectionsIterator(PrivateConstructor, Args&&...args) :
  impl(spimpl::make_impl<Implementation>(std::forward<Args>(args)...)) {}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const SaddleConnections<Surface>&) {
  return os << "SaddleConnections()";
}
}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION)(LIBFLATSURF_INSTANTIATE_HASH), SaddleConnections, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
