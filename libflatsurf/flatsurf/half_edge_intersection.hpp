/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2021 Julian Rüth
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

#ifndef LIBFLATSURF_HALF_EDGE_INTERSECTION_HPP
#define LIBFLATSURF_HALF_EDGE_INTERSECTION_HPP

#include <boost/operators.hpp>
#include <iosfwd>
#include <optional>

#include "copyable.hpp"

namespace flatsurf {

/// The intersection of a saddle connection and a half edge.
template <typename Surface>
class HalfEdgeIntersection : boost::less_than_comparable<HalfEdgeIntersection<Surface>> {
  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

  using T = typename Surface::Coordinate;

  template <typename... Args>
  HalfEdgeIntersection(PrivateConstructor, Args&&... args);

 public:
  // Return the approximate point where this crosses the half edge as a number
  // in `[0, 1]`, correct to double precision.
  double at() const;

  // Return the half edge this crosses.
  HalfEdge halfEdge() const;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const HalfEdgeIntersection<S>&);

  // Return whether this intersection crosses a half edge closer to its start
  // than `rhs`.
  // This operator is consistent with `at`, i.e., if `a.at() < b.at()`, then `a < b`.
  bool operator<(const HalfEdgeIntersection& rhs) const;

 private:
  Copyable<HalfEdgeIntersection> self;

  friend ImplementationOf<HalfEdgeIntersection>;
};

}  // namespace flatsurf

#endif
