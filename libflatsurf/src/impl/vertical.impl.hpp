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

#ifndef LIBFLATSURF_VERTICAL_IMPL_HPP
#define LIBFLATSURF_VERTICAL_IMPL_HPP

#include <functional>
#include <memory>

#include "../../flatsurf/vertical.hpp"

namespace flatsurf {

template <typename Surface>
class Implementation<Vertical<Surface>> {
  using Vertical = ::flatsurf::Vertical<Surface>;
  using T = typename Surface::Coordinate;

 public:
  Implementation(std::shared_ptr<const Surface>, const Vector<T>&);

  static bool visit(const Vertical& self, HalfEdge start, std::set<HalfEdge>& component, std::function<bool(HalfEdge)> visitor);

  std::shared_ptr<const Surface> surface;
  Vector<T> vertical;
  Vector<T> horizontal;
};

}  // namespace flatsurf

#endif
