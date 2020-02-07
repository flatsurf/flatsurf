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

#ifndef LIBFLATSURF_SADDLE_CONNECTION_IMPL_HPP
#define LIBFLATSURF_SADDLE_CONNECTION_IMPL_HPP

#include "../../flatsurf/chain.hpp"
#include "../../flatsurf/flat_triangulation.hpp"
#include "../../flatsurf/flat_triangulation_collapsed.hpp"
#include "../../flatsurf/saddle_connection.hpp"

namespace flatsurf {

template <typename Surface>
class Implementation<SaddleConnection<Surface>> {
  using SaddleConnection = flatsurf::SaddleConnection<Surface>;
  using T = typename Surface::Coordinate;

 public:
  Implementation(std::shared_ptr<const Surface> &surface, HalfEdge source, HalfEdge target, const Chain<Surface> &vector);

  static void check(const SaddleConnection &);

  void normalize();

  std::shared_ptr<const Surface> surface;
  HalfEdge source;
  HalfEdge target;
  Chain<Surface> chain;
};

}  // namespace flatsurf
#endif
