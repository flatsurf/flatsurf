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

#ifndef LIBFLATSURF_SADDLE_CONNECTIONS_HPP
#define LIBFLATSURF_SADDLE_CONNECTIONS_HPP

#include <boost/iterator/iterator_facade.hpp>
#include <memory>

#include "external/spimpl/spimpl.h"
#include "half_edge.hpp"
#include "vertex.hpp"

namespace flatsurf {
template <typename Surface>
class SaddleConnections {
  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

  using T = typename Surface::Coordinate;

 public:
  using Iterator = SaddleConnectionsIterator<Surface>;

  // All saddle connections on the surface starting at any vertex.
  SaddleConnections(const std::shared_ptr<const Surface> &, Bound searchRadius);

  // All saddle connections on the surface starting at source.
  SaddleConnections(const std::shared_ptr<const Surface> &, Bound searchRadius, const Vertex source);

  // The saddle connections that are starting at the source of sectorBegin
  // and lie in the sector between sectorBegin (inclusive) and the following
  // half edge (exclusive) in counter-clockwise order.
  SaddleConnections(const std::shared_ptr<const Surface> &, Bound searchRadius, HalfEdge sectorBegin);

  SaddleConnectionsIterator<Surface> begin() const;
  SaddleConnectionsIterator<Surface> end() const;

  template <typename Surf>
  friend std::ostream &operator<<(std::ostream &, const SaddleConnections<Surf> &);

 private:
  using Implementation = ImplementationOf<SaddleConnections>;
  friend Implementation;
  spimpl::impl_ptr<Implementation> impl;
};

template <typename Surface>
SaddleConnections(const std::shared_ptr<Surface> &, Bound) -> SaddleConnections<Surface>;

template <typename Surface>
SaddleConnections(const std::shared_ptr<Surface> &, Bound, const Vertex &) -> SaddleConnections<Surface>;

template <typename Surface>
SaddleConnections(const std::shared_ptr<Surface> &, Bound, const HalfEdge &) -> SaddleConnections<Surface>;

}  // namespace flatsurf

#endif
