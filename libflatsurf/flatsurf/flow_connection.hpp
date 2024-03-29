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

#ifndef LIBFLATSURF_FLOW_CONNECTION_HPP
#define LIBFLATSURF_FLOW_CONNECTION_HPP

#include <boost/operators.hpp>
#include <iosfwd>

#include "copyable.hpp"

namespace flatsurf {

// A saddle connection on the perimeter of a Flow Component.
template <typename Surface>
class FlowConnection : Serializable<FlowConnection<Surface>>,
                       boost::equality_comparable<FlowConnection<Surface>> {
  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type parameter must not have modifiers such as const");

  using T = typename Surface::Coordinate;

  // Flow connections cannot be created directly (other than copying & moving them.)
  // They are created as products of FlowDecomposition.
  template <typename... Args>
  FlowConnection(PrivateConstructor, Args&&... args);

 public:
  FlowComponent<Surface> component() const;

  FlowConnection<Surface> operator-() const;

  FlowConnection<Surface> nextInPerimeter() const;

  FlowConnection<Surface> previousInPerimeter() const;

  // Return whether this is a vertical (parallel or antiparallel) connection
  // with respect to the fixed vertical direction.
  bool vertical() const;

  bool parallel() const;

  bool antiparallel() const;

  // Return whether this is a non-vertical connection in the top contour.
  bool top() const;

  // Return whether this is a non-vertical connection in the bottom contour.
  bool bottom() const;

  // Return whether this edge is a (vertical) boundary between two distinct FlowComponents.
  bool boundary() const;

  bool operator==(const FlowConnection<Surface>&) const;

  SaddleConnection<FlatTriangulation<T>> saddleConnection() const;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const FlowConnection<S>&);

 private:
  Copyable<FlowConnection> self;

  friend ImplementationOf<FlowConnection>;
};
}  // namespace flatsurf

namespace std {

template <typename Surface>
struct hash<::flatsurf::FlowConnection<Surface>> { std::size_t operator()(const ::flatsurf::FlowConnection<Surface>&) const; };

}  // namespace std

#endif
