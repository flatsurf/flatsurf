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

#ifndef LIBFLATSURF_SADDLE_CONNECTION_IMPL_HPP
#define LIBFLATSURF_SADDLE_CONNECTION_IMPL_HPP

#include "../../flatsurf/chain.hpp"
#include "../../flatsurf/flat_triangulation.hpp"
#include "../../flatsurf/flat_triangulation_collapsed.hpp"
#include "../../flatsurf/saddle_connection.hpp"

#include "read_only.hpp"
#include "flat_triangulation.impl.hpp"

namespace flatsurf {

template <typename Surface>
class ImplementationOf<SaddleConnection<Surface>> {
  using SaddleConnection = flatsurf::SaddleConnection<Surface>;
  using T = typename Surface::Coordinate;

 public:
  ImplementationOf(const Surface&, HalfEdge source, HalfEdge target, const Chain<Surface>&);
  ImplementationOf(const Surface&, HalfEdge source, HalfEdge target, Chain<Surface>&&);

  ReadOnly<Surface> surface;
  HalfEdge source;
  HalfEdge target;
  Chain<Surface> chain;
};

}  // namespace flatsurf
#endif
