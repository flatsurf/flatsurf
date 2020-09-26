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

#ifndef LIBFLATSURF_CONTOUR_DECOMPOSITION_HPP
#define LIBFLATSURF_CONTOUR_DECOMPOSITION_HPP

#include <vector>

#include "movable.hpp"

namespace flatsurf {

// The decomposition of a (Collapsed) Flat Triangulation into Contour
// Components, i.e., components whose graph of faces is connected when only
// considering faces adjacent which share a non-vertical edge.
// Note that further edges in each component have been flipped such that there
// is only a single large edge in each component.
template <class Surface>
class ContourDecomposition {
  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

  using T = typename Surface::Coordinate;

 public:
  ContourDecomposition(Surface, const Vector<T> &vertical);

  std::vector<ContourComponent<Surface>> components() const;

  // Return the underlying Collapsed Flat Triangulation where all edges have
  // been flipped such that each Contour Component has a unique large edge.
  const FlatTriangulationCollapsed<T> &collapsed() const;

  template <typename S>
  friend std::ostream &operator<<(std::ostream &, const ContourDecomposition<S> &);

 private:
  Movable<ContourDecomposition> self;

  friend ImplementationOf<ContourDecomposition>;
};

}  // namespace flatsurf

#endif
