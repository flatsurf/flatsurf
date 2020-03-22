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

#ifndef LIBFLATSURF_SADDLE_CONNECTIONS_IMPL_HPP
#define LIBFLATSURF_SADDLE_CONNECTIONS_IMPL_HPP

#include "../../flatsurf/saddle_connections.hpp"
#include "../../flatsurf/bound.hpp"

namespace flatsurf {

template <typename Surface>
class ImplementationOf<SaddleConnections<Surface>> {
 public:
  ImplementationOf(std::shared_ptr<const Surface>, Bound, const std::vector<HalfEdge>&);

  std::shared_ptr<const Surface> surface;
  Bound bound;
  std::vector<HalfEdge> sectors;
};

}

#endif
