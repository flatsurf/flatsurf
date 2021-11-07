/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020 Julian Rüth
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

#ifndef LIBFLATSURF_SADDLE_CONNECTIONS_SAMPLE_HPP
#define LIBFLATSURF_SADDLE_CONNECTIONS_SAMPLE_HPP

#include <iosfwd>
#include <optional>

#include "copyable.hpp"

namespace flatsurf {

// The sequence of Saddle Connections on a triangulation translation surface
// randomly sampled.
// The distribution of the randomly selected connections should be considered
// an implementation detail but is probably nothing that can be relied on for
// statistical purposes. Roughly, we walk the triangulation randomly and return
// saddle connections as we see them. This process gives a strong preference to
// short connections (in particular initially.)
template <typename Surface>
class SaddleConnectionsSample {
  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

  using T = typename Surface::Coordinate;

  template <typename... Args>
  SaddleConnectionsSample(PrivateConstructor, Args &&... args);

 public:
  SaddleConnectionsSample(const SaddleConnections<Surface> &);

  // Return the configured lower bound.
  std::optional<Bound> lowerBound() const;

  // Return only saddle connections whose length exceeds the lower bound.
  SaddleConnectionsSample lowerBound(Bound) const;

  // Return the configured bound.
  std::optional<Bound> bound() const;

  // Return only saddle connections whose length does not exceed the bound.
  SaddleConnectionsSample bound(Bound) const;

  // Return only the saddle connections starting at the source of sectorBegin
  // that lie in the sector between sectorBegin (inclusive) and the following
  // half edge (exclusive) in counter-clockwise order.
  SaddleConnectionsSample sector(HalfEdge sectorBegin) const;

  // Return only the saddle connections starting at source of sectorBegin that
  // lie in sector between sectorBegin (inclusive) and sectorEnd (exclusive.)
  SaddleConnectionsSample sector(const SaddleConnection<Surface> &sectorBegin, const SaddleConnection<Surface> &sectorEnd) const;

  // Return only the saddle connections whose directions lies in sector between
  // sectorBegin (inclusive) and sectorEnd (exclusive.)
  SaddleConnectionsSample sector(const Vector<T> &sectorBegin, const Vector<T> &sectorEnd) const;

  // Return only the saddle connections starting at source.
  SaddleConnectionsSample source(const Vertex &source) const;

  // Return the saddle connections ordered by increasing angle.
  SaddleConnections<Surface> byAngle() const;

  // Return the saddle connections sorted by increasing length.
  SaddleConnectionsByLength<Surface> byLength() const;

  const Surface &surface() const;

  using iterator = SaddleConnectionsSampleIterator<Surface>;

  // Return an iterator through the saddle connections. The iteration is
  // counterclockwise around each vertex.
  iterator begin() const;
  // End position of the iterator through the saddle connections.
  iterator end() const;

  template <typename S>
  friend std::ostream &operator<<(std::ostream &, const SaddleConnectionsSample<S> &);

 private:
  Copyable<SaddleConnectionsSample> self;

  friend ImplementationOf<SaddleConnectionsSample>;
  friend iterator;
  friend ImplementationOf<iterator>;
};

template <typename Surface>
SaddleConnectionsSample(const Surface &) -> SaddleConnectionsSample<Surface>;

}  // namespace flatsurf

#endif
