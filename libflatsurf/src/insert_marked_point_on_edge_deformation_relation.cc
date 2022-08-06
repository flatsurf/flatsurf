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

#include "impl/insert_marked_point_on_edge_deformation_relation.hpp"

#include <ostream>

#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/point.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/half_edge_set.hpp"
#include "../flatsurf/half_edge_set_iterator.hpp"
#include "../flatsurf/vector.hpp"
#include "impl/generic_retriangulation_deformation_relation.hpp"
#include "impl/point.impl.hpp"
#include "util/assert.ipp"

namespace flatsurf {

template <typename Surface>
InsertMarkedPointOnEdgeDeformationRelation<Surface>::InsertMarkedPointOnEdgeDeformationRelation(const Surface& domain, const Surface& codomain, const Vertex& inserted, HalfEdge split, HalfEdge a, HalfEdge b) :
  DeformationRelation<Surface>(domain, codomain), inserted(inserted), split(split), a(a), b(b) {
  LIBFLATSURF_ASSERT_ARGUMENT(codomain.fromHalfEdge(a) + codomain.fromHalfEdge(b) == domain.fromHalfEdge(split), "inserted vertex does not divide split half edge since " << split << " != " << a << " + " << b << " since " << domain.fromHalfEdge(split) << " != " << codomain.fromHalfEdge(a) << " + " << codomain.fromHalfEdge(b));
  LIBFLATSURF_ASSERT_ARGUMENT(Vertex::target(a, codomain) == inserted, "Inserted vertex not at the end of HalfEdge " << a);
  LIBFLATSURF_ASSERT_ARGUMENT(Vertex::source(b, codomain) == inserted, "Inserted vertex not at the start of HalfEdge " << b);
}

template <typename Surface>
std::optional<Path<Surface>> InsertMarkedPointOnEdgeDeformationRelation<Surface>::operator()(const Path<Surface>&) const {
  throw std::logic_error("not implemented: cannot map paths when a vertex was inserted in the interior of an existing edge");
}

template <typename Surface>
Point<Surface> InsertMarkedPointOnEdgeDeformationRelation<Surface>::operator()(const Point<Surface>& point) const {
  const auto V = this->point();

  auto face = point.face();

  if (!V.in(face)) {
    // The barycentric coordinates of the point remain valid in the codomain
    // since the inserted vertex has no influence on the face where the point
    // is defined.
    return point;
  }

  // Write the point as ap A + bp B + cp C for a face (A, B, C) that contains
  // the point. In the codomain, (A, B, C) do not form a face anymore. The
  // point V was inserted on one of the edges. We rotate so that it was
  // inserted on ``face``.
  while (!V.on(face))
      face = this->domain->nextInFace(face);

  const auto [ap, bp, cp] = point.coordinates(face);

  const auto AB = face;
  const auto BC = this->domain->nextInFace(AB);
  const auto CA = this->domain->nextInFace(BC);
  const auto VC = this->codomain->previousInFace(CA);

  {
    // We write A in terms of (C, V, B)
    auto A = ImplementationOf<Point<Surface>>{*this->codomain, CA, T(), T(1), T()};
    A.rotate(VC);
    const auto [ca, va, ba] = A.crossed();

    // Write the point in terms of (C, V, B)
    const T c_p = ap * ca + cp * (ca + va + ba);
    const T v_p = ap * va;
    const T b_p = ap * ba + bp * (ca + va + ba);

    if (c_p >= 0 && v_p >= 0 && b_p >= 0)
      return Point{*this->codomain, -VC, c_p, v_p, b_p};
  }

  {
    // We write B in terms of (V, C, A).
    auto B = ImplementationOf<Point<Surface>>{*this->codomain, BC, T(1), T(), T()};
    B.rotate(-VC);
    const auto [vb, cb, ab] = B.crossed();

    // Write the point in terms of (V, C, A)
    const T v_p = bp * vb;
    const T c_p = bp * cb + cp * (vb + cb + ab);
    const T a_p = bp * ab + ap * (vb + cb + ab);

    if (v_p >= 0 && c_p >= 0 && a_p >= 0)
      return Point{*this->codomain, VC, v_p, c_p, a_p};
  }

  LIBFLATSURF_UNREACHABLE("Point " << point << " had no non-negative barycentric coordinates in any of the faces adjacent to " << VC); 
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> InsertMarkedPointOnEdgeDeformationRelation<Surface>::clone() const {
  return std::make_unique<InsertMarkedPointOnEdgeDeformationRelation>(this->domain, this->codomain, inserted, split, a, b);
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> InsertMarkedPointOnEdgeDeformationRelation<Surface>::section() const {
  // There is no specialized EraseMarkedPointDeformation implemented yet, so we use a generic RetriangulationDeformationRelation instead.
  return std::make_unique<GenericRetriangulationDeformationRelation<Surface>>(this->codomain, this->domain, this->codomain->nextAtVertex(a), this->domain->nextAtVertex(split));
}

template <typename Surface>
bool InsertMarkedPointOnEdgeDeformationRelation<Surface>::trivial() const {
  return false;
}

template <typename Surface>
Point<Surface> InsertMarkedPointOnEdgeDeformationRelation<Surface>::point() const {
  return Point{*this->domain, split, this->codomain->fromHalfEdge(a)};
}

template <typename Surface>
std::ostream& InsertMarkedPointOnEdgeDeformationRelation<Surface>::operator>>(std::ostream& os) const {
  return os << this->domain << " → " << this->codomain << " inserting " << this->inserted;
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_STATIC), InsertMarkedPointOnEdgeDeformationRelation, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
