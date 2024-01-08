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

#include "impl/insert_marked_point_in_face_deformation_relation.hpp"

#include <ostream>

#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/point.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/half_edge_set.hpp"
#include "../flatsurf/half_edge_set_iterator.hpp"
#include "impl/point.impl.hpp"
#include "impl/generic_retriangulation_deformation_relation.hpp"
#include "util/assert.ipp"

namespace flatsurf {

template <typename Surface>
InsertMarkedPointInFaceDeformationRelation<Surface>::InsertMarkedPointInFaceDeformationRelation(const Surface& domain, const Surface& codomain, const Vertex& inserted) :
  DeformationRelation<Surface>(domain, codomain), inserted(inserted) {}

template <typename Surface>
std::optional<Path<Surface>> InsertMarkedPointInFaceDeformationRelation<Surface>::operator()(const Path<Surface>& path) const {
  // A path in the domain, essentially described as a sequence of half edges,
  // remain valid when inserting a vertex in the interior of the face.
  return path;
}

template <typename Surface>
Point<Surface> InsertMarkedPointInFaceDeformationRelation<Surface>::operator()(const Point<Surface>& point) const {
  const auto V = this->point();

  auto face = point.face();

  if (!V.in(face)) {
    // The barycentric coordinates of the point remain valid in the codomain
    // since the inserted vertex has no influence on the face where the point
    // is defined.
    return point;
  }

  for (const auto& AB : {face, this->domain->nextInFace(face), this->domain->previousInFace(face)}) {
    // Write the point as ap A + bp B + cp C for a face (A, B, C) that contains
    // the point.
    const auto [ap, bp, cp] = point.coordinates(face);

    // In the codomain (A, B, C) do not form a face anymore. The point V was
    // inserted as a vertex in the interior of that face.
    
    // Write A in coordinates of (V, B, C).
    auto A = ImplementationOf<Point<Surface>>{*this->codomain, AB, T(1), T(), T()};
    const auto BV = this->codomain->nextInFace(AB);
    A.rotate(BV);
    const auto [va, ba, ca] = A.crossed();

    // Write the point in terms of (V, B, C)
    const T v_p = ap * va;
    const T b_p = ap * ba + bp * (va + ba + ca);
    const T c_p = ap * ca + cp * (va + ba + ca);

    if (v_p >= 0 && b_p >= 0 && c_p >= 0)
      return Point{*this->codomain, -BV, v_p, b_p, c_p};
  }

  LIBFLATSURF_UNREACHABLE("Point " << point << " had no non-negative barycentric coordinates in any of the faces surrounding the inserted vertex " << V); 
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> InsertMarkedPointInFaceDeformationRelation<Surface>::clone() const {
  return std::make_unique<InsertMarkedPointInFaceDeformationRelation>(this->domain, this->codomain, inserted);
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> InsertMarkedPointInFaceDeformationRelation<Surface>::section() const {
  // There is no specialized EraseMarkedPointDeformation implemented yet, so we use a generic RetriangulationDeformationRelation instead.
  // All the half edges that exist in the domain are unchanged.
  return std::make_unique<GenericRetriangulationDeformationRelation<Surface>>(this->codomain, this->domain, HalfEdge(1), HalfEdge(1));
}

template <typename Surface>
bool InsertMarkedPointInFaceDeformationRelation<Surface>::trivial() const {
  return false;
}

template <typename Surface>
Point<Surface> InsertMarkedPointInFaceDeformationRelation<Surface>::point() const {
  // A vertex V was inserted into a face (A, B, C) in the domain.
  // It's coordinates are the Cartesian coordinates of the half edge (A, V) in
  // the codomain.
  const auto AV = *inserted.incoming().begin();
  const auto AB = this->codomain->previousAtVertex(AV);
  return Point{*this->domain, AB, this->codomain->fromHalfEdge(AV)};
}

template <typename Surface>
std::ostream& InsertMarkedPointInFaceDeformationRelation<Surface>::operator>>(std::ostream& os) const {
  return os << this->domain << " → " << this->codomain << " inserting " << this->inserted;
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_STATIC), InsertMarkedPointInFaceDeformationRelation, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
