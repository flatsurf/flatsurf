/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2022 Julian Rüth
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

#ifndef LIBFLATSURF_SEGMENT_HPP
#define LIBFLATSURF_SEGMENT_HPP

#include <optional>

#include <boost/operators.hpp>

#include "bound.hpp"
#include "copyable.hpp"
#include "serializable.hpp"

namespace flatsurf {

// An oriented segment connection two points in a surface.
template <typename Surface>
class Segment : public Serializable<Segment<Surface>>,
                boost::equality_comparable<Segment<Surface>> {
  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

  using T = typename Surface::Coordinate;

 public:
  Segment(const Point<Surface>& start, const Point<Surface>& end, const Vector<T>&);
  Segment(const Point<Surface>& start, HalfEdge source, const Point<Surface>& end, HalfEdge target, const Vector<T>&);

  // Return the starting point of this segment.
  Point<Surface> start() const;

  // Return the end point of this segment.
  Point<Surface> end() const;

  // Return a half edge such that its face contains start() and the initial
  // part of the vector() to end().
  HalfEdge source() const;

  // Return a half edge such that its face contains end() and the final part of
  // the vector() from start().
  HalfEdge target() const;

  // Return the vector end - start describing this segment.
  const Vector<T>& vector() const;

  operator const Vector<T>&() const;

  // Return the saddle connection this segment represents, if any.
  std::optional<SaddleConnection<Surface>> saddleConnection() const;

  Segment<Surface> operator-() const;

  bool operator==(const Segment<Surface>&) const;

  template <typename S>
  friend std::ostream &operator<<(std::ostream &, const Segment<S> &);

 private:
  Copyable<Segment> self;

  friend ImplementationOf<Segment>;
};

template <typename Surface, typename... T>
Segment(const Surface &, T &&...) -> Segment<Surface>;

}

namespace std {

template <typename Surface>
struct hash<::flatsurf::Segment<Surface>> { size_t operator()(const ::flatsurf::Segment<Surface> &) const; };

}  // namespace std

#endif
