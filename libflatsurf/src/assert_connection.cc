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

#include "impl/assert_connection.hpp"

#include <iostream>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/ccw.hpp"
#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/saddle_connections_iterator.hpp"
#include "../flatsurf/vector.hpp"

namespace flatsurf {

template <typename T>
bool AssertConnection<T>::operator()(const SaddleConnection<FlatTriangulation<T>>& claimed) {
  if (cost > budget) {
    budget++;
    return true;
  }

  const Vector<T> v = claimed;
  const auto connections = SaddleConnections<FlatTriangulation<T>>(claimed.surface()).sector(claimed.source()).bound(Bound::upper(v));

  auto it = std::begin(connections);

  long long steps = 1;

  for (it = std::begin(connections); it != std::end(connections); ++it, ++steps) {
    if (--budget == 0) {
      // We could not trace this connection in the expected cost. Abort and
      // assume that we need twice as long for a typical connection.
      cost *= 2;
      return true;
    }

    const Vector<T> w = *it;
    switch (w.ccw(v)) {
      case CCW::COLLINEAR:
        cost = steps;
        return w == v;
      case CCW::CLOCKWISE:
        it.skipSector(CCW::COUNTERCLOCKWISE);
        break;
      case CCW::COUNTERCLOCKWISE:
        it.skipSector(CCW::CLOCKWISE);
        break;
    }
  }

  return false;
}

}  // namespace flatsurf

#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_STATIC), AssertConnection, LIBFLATSURF_REAL_TYPES)
