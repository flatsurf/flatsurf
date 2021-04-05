/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020-2021 Julian Rüth
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

#ifndef LIBFLATSURF_SADDLE_CONNECTIONS_ITERATOR_HPP
#define LIBFLATSURF_SADDLE_CONNECTIONS_ITERATOR_HPP

#include <boost/iterator/iterator_facade.hpp>
#include <optional>

#include "copyable.hpp"

namespace flatsurf {

// Iterates over the saddle connections on a triangulation translation surface.
template <typename Surface>
class SaddleConnectionsIterator : public boost::iterator_facade<SaddleConnectionsIterator<Surface>, const SaddleConnection<Surface>, boost::forward_traversal_tag> {
  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

  using T = typename Surface::Coordinate;

  template <typename... Args>
  SaddleConnectionsIterator(PrivateConstructor, Args &&... args);

 public:
  // Advance the iterator to the next saddle connection.
  void increment();

  // Advance the iterator to the next saddle connection or until a HalfEdge is
  // being crossed during the search (in forward direction.) This can be useful
  // if information about the exact path in the surface for a saddle connection
  // needs to be reconstructed.
  [[deprecated("Use incrementWithIntersections() instead.")]]
  std::optional<HalfEdge> incrementWithCrossings();

  // Advance the iterator to the next saddle connection or until a HalfEdge is
  // being crossed during the search (in forward direction.) This can be useful
  // if information about the exact path in the surface for a saddle connection
  // needs to be reconstructed.
  // Note that the iterator is initially starting on a saddle connection. The
  // intersections reported by this method do not include the ones that were
  // taken to get to that initial saddle connection, i.e., setting a
  // lower/upper bound or a sector that excludes the source half edge, will
  // lead to missing initial intersections.
  std::optional<std::pair<Chain<Surface>, HalfEdge>> incrementWithIntersections();

  bool equal(const SaddleConnectionsIterator &other) const;

  const SaddleConnection<Surface> &dereference() const;

  void skipSector(CCW sector);

  template <typename S>
  friend std::ostream &operator<<(std::ostream &, const SaddleConnectionsIterator<S> &);

 private:
  Copyable<SaddleConnectionsIterator> self;

  friend SaddleConnections<Surface>;
  friend ImplementationOf<SaddleConnectionsIterator>;
};

}  // namespace flatsurf

#endif
