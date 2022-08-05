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

#include "impl/insert_marked_deformation_relation.hpp"

#include <ostream>

#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/point.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/half_edge_set.hpp"
#include "../flatsurf/half_edge_set_iterator.hpp"
#include "impl/point.impl.hpp"
#include "util/assert.ipp"

namespace flatsurf {

template <typename Surface>
InsertMarkedDeformationRelation<Surface>::InsertMarkedDeformationRelation(const Surface& domain, const Surface& codomain, const Vertex& inserted) :
  DeformationRelation<Surface>(domain, codomain), inserted(inserted) {}

template <typename Surface>
InsertMarkedDeformationRelation<Surface>::InsertMarkedDeformationRelation(const Surface& domain, const Surface& codomain, const Vertex& inserted, HalfEdge, HalfEdge, HalfEdge) :
  DeformationRelation<Surface>(domain, codomain), inserted(inserted) {}

template <typename Surface>
std::optional<Path<Surface>> InsertMarkedDeformationRelation<Surface>::operator()(const Path<Surface>& path) const {
  if (face())
    // A path in the domain, essentially described as a sequence of half edges,
    // remain valid when inserting a vertex in the interior of the face.
    return path;

  throw std::logic_error("not implemented: cannot map paths when a vertex was inserted in the interior of an existing edge");
}

template <typename Surface>
Point<Surface> InsertMarkedDeformationRelation<Surface>::operator()(const Point<Surface>& point) const {
  const auto V = this->point();

  auto face = point.face();

  if (!V.in(face)) {
    // The barycentric coordinates of the point remain valid in the codomain
    // since the inserted vertex has no influence on the face where the point
    // is defined.
    return point;
  }

  if (this->face()) {
    for (const auto AB : {face, this->domain->nextInFace(face), this->domain->previousInFace(face)}) {
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
  } else {
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
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> InsertMarkedDeformationRelation<Surface>::clone() const {
  return std::make_unique<InsertMarkedDeformationRelation>(this->domain, this->codomain, inserted);
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> InsertMarkedDeformationRelation<Surface>::section() const {
  throw std::logic_error("not implemented: cannot compute section of an insertion of a marked point");
}

template <typename Surface>
bool InsertMarkedDeformationRelation<Surface>::trivial() const {
  return false;
}

template <typename Surface>
bool InsertMarkedDeformationRelation<Surface>::face() const {
  return this->codomain->atVertex(inserted).size() == 3;
}

template <typename Surface>
Point<Surface> InsertMarkedDeformationRelation<Surface>::point() const {
  if (face()) {
    // The vertex V was inserted into the face (A, B, C).
    // It's coordinates are the Cartesian coordinates of the half edge (A, V).
    const auto AV = *inserted.incoming().begin();
    const auto AB = this->codomain->previousInFace(-AV);
    return Point{*this->domain, AB, this->codomain->fromHalfEdge(AV)};
  } else {
    throw std::logic_error("not implemented: point() when vertex was inserted on an existing half edge");
  }
}

template <typename Surface>
std::ostream& InsertMarkedDeformationRelation<Surface>::operator>>(std::ostream& os) const {
  return os << this->domain << " → " << this->codomain << " inserting " << this->inserted;
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_STATIC), InsertMarkedDeformationRelation, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
