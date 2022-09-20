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

#ifndef LIBFLATSURF_PATH_IMPL_HPP
#define LIBFLATSURF_PATH_IMPL_HPP

#include "../../flatsurf/path.hpp"

#include "../../flatsurf/segment.hpp"

namespace flatsurf {

template <typename Surface>
class ImplementationOf<Path<Surface>> {
  using T = typename Surface::Coordinate;

 public:
  ImplementationOf();
  ImplementationOf(const std::vector<Segment<Surface>>&);

  std::vector<Segment<Surface>> path = {};

  // These methods can be removed once Path::begin() and Path::end() have been
  // changed to return a SegmentIterator.
  SegmentIterator<Surface> begin();
  SegmentIterator<Surface> end();

  // This cache exists to make the cast to vector<SaddleConnection>& work. It
  // can be removed once that cast operator has been dropped.
  mutable std::optional<std::vector<SaddleConnection<Surface>>> pathAsSaddleConnections = std::nullopt;

  static bool connected(const Segment<Surface>&, const Segment<Surface>&);
};

}  // namespace flatsurf

#endif
