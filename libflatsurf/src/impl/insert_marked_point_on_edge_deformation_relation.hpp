/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2021-2022 Julian Rüth
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

#ifndef LIBFLATSURF_IMPL_INSERT_MARKED_POINT_ON_EDGE_DEFORMATION_RELATION_HPP
#define LIBFLATSURF_IMPL_INSERT_MARKED_POINT_ON_EDGE_DEFORMATION_RELATION_HPP

#include <iosfwd>

#include "../../flatsurf/vertex.hpp"
#include "../../flatsurf/half_edge.hpp"
#include "deformation_relation.hpp"

namespace flatsurf {

// The deformation that inserts a marked point in the interior of a half edge
// `split`.  The marked point is then attached to four half edges, two of them
// sum as `split = a + b`.
// Note that ``GenericRetriangulationDeformationRelation`` generalizes this
// deformation. We could always use
// ``GenericRetriangulationDeformationRelation`` instead but hopefully this
// implementation is more efficient.
template <typename Surface>
class InsertMarkedPointOnEdgeDeformationRelation : public DeformationRelation<Surface> {
  using T = typename Surface::Coordinate;

 public:
  InsertMarkedPointOnEdgeDeformationRelation(const Surface& domain, const Surface& codomain, const Vertex& inserted, HalfEdge split, HalfEdge a, HalfEdge b);

  std::optional<Path<Surface>> operator()(const Path<Surface>&) const override;
  Point<Surface> operator()(const Point<Surface>&) const override;

  std::unique_ptr<DeformationRelation<Surface>> clone() const override;

  std::unique_ptr<DeformationRelation<Surface>> section() const override;

  bool trivial() const override;

  // Return the inserted vertex as an element of the domain.
  Point<Surface> point() const;

  std::ostream& operator>>(std::ostream&) const override;

  Vertex inserted;
  HalfEdge split;
  HalfEdge a;
  HalfEdge b;
};

}  // namespace flatsurf

#endif
