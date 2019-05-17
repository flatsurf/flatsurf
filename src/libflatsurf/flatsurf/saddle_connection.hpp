/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
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

#ifndef LIBFLATSURF_SADDLE_CONNECTION_HPP
#define LIBFLATSURF_SADDLE_CONNECTION_HPP

#include <boost/operators.hpp>
#include <iosfwd>
#include "external/spimpl/spimpl.h"

#include "flatsurf/flatsurf.hpp"
#include "flatsurf/forward.hpp"
#include "flatsurf/half_edge.hpp"

namespace flatsurf {
template <typename Vector>
struct FlatTriangulation;

struct Vertex;

template <typename Vector, typename AlongTriangulation>
struct SaddleConnection : boost::equality_comparable<SaddleConnection<Vector, AlongTriangulation>> {
  const AlongTriangulation &vector() const;
  // The saddle connection is leaving from the vertex at the source of source.
  // It is leaving in a direction that is contained in the sector next to
  // source (counterclockwise.)
  HalfEdge source() const;
  // The saddle connection is reaching the vertex at the target of target.
  // It is approching in a direction that is contained in the sector
  // *clockwise* from *-target*.
  HalfEdge target() const;

  bool operator==(const SaddleConnection<Vector, AlongTriangulation> &) const;

  template <typename V, typename A>
  friend std::ostream &operator<<(std::ostream &, const SaddleConnection<V, A> &);

 private:
  friend SaddleConnectionsIterator<Vector, AlongTriangulation>;

  struct Implementation;
  spimpl::impl_ptr<Implementation> impl;
};
}  // namespace flatsurf

#endif
