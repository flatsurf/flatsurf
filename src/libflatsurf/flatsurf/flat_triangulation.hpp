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

#ifndef LIBFLATSURF_FLAT_TRIANGULATION_HPP
#define LIBFLATSURF_FLAT_TRIANGULATION_HPP

#include <iosfwd>
#include <memory>
#include <vector>

#include "external/spimpl/spimpl.h"

#include "flatsurf/flat_triangulation_combinatorial.hpp"
#include "flatsurf/forward.hpp"

namespace flatsurf {
template <class T>
class FlatTriangulation : public FlatTriangulationCombinatorial {
 public:
  using Vector = flatsurf::Vector<T>;

  FlatTriangulation(FlatTriangulationCombinatorial &&, const std::vector<Vector> &vectors);
  FlatTriangulation(FlatTriangulationCombinatorial &&, HalfEdgeMap<Vector> &&vectors);

  // Create an unrelated clone of this triangulation that is built from the
  // same data. There is no copy-constructor since it is too likely that
  // this is would not update the associated HalfEdgeMaps in the way that the
  // caller expects.
  FlatTriangulation<T> clone() const;

  T area() const noexcept;

  const Vector &fromEdge(HalfEdge) const;

  template <typename W>
  friend std::ostream &operator<<(std::ostream &, const FlatTriangulation<W> &);

 private:
  class Implementation;
  spimpl::unique_impl_ptr<Implementation> impl;
};

template <typename Vector>
FlatTriangulation(const std::vector<std::vector<int>> &, const std::vector<Vector> &)->FlatTriangulation<typename Vector::Coordinate>;
}  // namespace flatsurf

#endif
