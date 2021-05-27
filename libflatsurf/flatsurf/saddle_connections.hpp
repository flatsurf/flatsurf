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

#include <optional>

#include "copyable.hpp"

namespace flatsurf {

// The sequence of Saddle Connections on a triangulation translation surface
// sorted by increasing angle around each vertex.
template <typename Surface>
class SaddleConnections {
  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

  using T = typename Surface::Coordinate;

 public:
  using ByLength = SaddleConnectionsByLength<Surface>;
  using ByAngle = SaddleConnections;
  using Sample = SaddleConnectionsSample<Surface>;

  SaddleConnections(const Surface &);

  // Return only the saddle connections whose length is at most the given bound.
  SaddleConnections<Surface> bound(Bound) const;

  // Return the configured bound of these saddle connections, if any.
  std::optional<Bound> bound() const;

  // Return only the saddle connections whose length is at most the given bound.
  SaddleConnections<Surface> lowerBound(Bound) const;

  // Return the configured bound of these saddle connections, if any.
  std::optional<Bound> lowerBound() const;

  // Return only the saddle connections starting at the source of sectorBegin
  // that lie in the sector between sectorBegin (inclusive) and the following
  // half edge (exclusive) in counter-clockwise order.
  SaddleConnections<Surface> sector(HalfEdge sectorBegin) const;

  // Return only the saddle connections starting at source of sectorBegin that
  // lie in sector between sectorBegin (inclusive) and sectorEnd (exclusive.)
  SaddleConnections<Surface> sector(const SaddleConnection<Surface> &sectorBegin, const SaddleConnection<Surface> &sectorEnd) const;

  // Return only the saddle connections whose directions lies in sector between
  // sectorBegin (inclusive) and sectorEnd (exclusive.)
  SaddleConnections<Surface> sector(const Vector<T> &sectorBegin, const Vector<T> &sectorEnd) const;

  // Return only the saddle connections starting at source.
  SaddleConnections<Surface> source(const Vertex &source) const;

  // Return the saddle connections around each source vertex sorted by
  // increasing angles (counterclockwise.) Note that this just returns this
  // object unchanged.
  SaddleConnection<Surface> &byAngle() const;

  // Return the saddle connections sorted by increasing length.
  ByLength byLength() const;

  // Return the saddle connections randomly sampled.
  Sample sample() const;

  const Surface &surface() const;

  using iterator = SaddleConnectionsIterator<Surface>;

  // Return an iterator through the saddle connections. The iteration is
  // counterclockwise around each vertex.
  iterator begin() const;
  // End position of the iterator through the saddle connections.
  iterator end() const;

  template <typename S>
  friend std::ostream &operator<<(std::ostream &, const SaddleConnections<S> &);

 private:
  Copyable<SaddleConnections> self;

  friend ImplementationOf<SaddleConnections>;
  friend iterator;
  friend ImplementationOf<iterator>;
  friend SaddleConnectionsByLength<Surface>;
  friend SaddleConnectionsSample<Surface>;
};

template <typename Surface>
SaddleConnections(const Surface &) -> SaddleConnections<Surface>;

}  // namespace flatsurf

#endif
