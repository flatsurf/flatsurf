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

#ifndef LIBFLATSURF_COMPONENT_HPP
#define LIBFLATSURF_COMPONENT_HPP

#include <optional>
#include <vector>
#include "external/spimpl/spimpl.h"

#include "flatsurf/forward.hpp"

namespace flatsurf {

template <typename T>
class Component {
 public:
  using Surface = FlatTriangulation<T>;
  using Boundary = std::vector<const SaddleConnection<Surface>>;

  bool cylinder() const noexcept;
  // Whether this is a cylinder or a certified minimal component.
  bool certified() const noexcept;
  // Maybe circular linked list? (On a cylinder, the right boundary goes with the vertical direction and the left against.)
  const std::vector<Boundary>& boundaries() const noexcept;
  FlatTriangulation<T> triangulation() const;
  // The real part of the non-vertical edges of the triangulation()
  LengthAlongTriangulation<T> width() const noexcept;
};

}  // namespace flatsurf

#endif
