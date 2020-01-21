/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Julian Rüth
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

#ifndef LIBFLATSURF_CONTOUR_CONNECTION_IMPL_HPP
#define LIBFLATSURF_CONTOUR_CONNECTION_IMPL_HPP

#include "../../flatsurf/contour_connection.hpp"
#include "../../flatsurf/half_edge.hpp"

#include "forward.hpp"

namespace flatsurf {

template <typename Surface>
class Implementation<ContourConnection<Surface>> {
 public:
  enum class Contour {
    BOTTOM = -1,
    TOP = 1,
  };

  Implementation(std::shared_ptr<ContourDecompositionState<Surface>>, const ContourComponent<Surface>& component, HalfEdge, Contour);

  static ContourConnection<Surface> make(std::shared_ptr<ContourDecompositionState<Surface>>, const ContourComponent<Surface>& component, HalfEdge);

  // Return the collapsed vertical connections that need to be crossed when
  // turning clockwise from from (i.e., nextInFace(from)) to to.
  static std::list<typename Surface::SaddleConnection> turn(const ContourConnection<Surface>& from, const ContourConnection<Surface>& to);

  // Return the collapsed vertical connections that need to be crossed when going from from to to.
  // The connections are split in two buckets of continuous connections, the
  // ones attached to from and the ones attached to to.
  // The connections are oriented such they all point towards the interior.
  static std::pair<std::list<typename Surface::SaddleConnection>,std::list<typename Surface::SaddleConnection>> cross(const ContourConnection<Surface>& from, const ContourConnection<Surface>& to);

  std::shared_ptr<ContourDecompositionState<Surface>> state;
  ContourComponent<Surface> component;
  // The normalized half edge describing this connection going from left to right.
  HalfEdge e;
  Contour contour;
};

}

#endif
