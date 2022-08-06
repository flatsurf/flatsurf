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

#ifndef LIBFLATSURF_IMPL_GENERIC_DEFORMATION_RELATION_HPP
#define LIBFLATSURF_IMPL_GENERIC_DEFORMATION_RELATION_HPP

#include <iosfwd>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../../flatsurf/saddle_connection.hpp"
#include "retriangulation_deformation_relation.hpp"

namespace flatsurf {

/// A deformation describing the relation between two surfaces, `domain` and
/// `codomain`, that are identical up to retriangulation and marked points.
/// There are specialized versions of this deformation relation, namely
/// ``RetriangulationDeformationRelation``,
/// ``InsertMarkedPointInFaceDeformationRelation``,
/// ``InsertMarkedPointInFaceDeformationRelation``. In principal, this
/// deformation could be built as a composition of these and their sections.
/// However, we sometimes create deformation through intermediate shifts which
/// have no effect on the final deformation. These shifts are necessary to
/// construct the codomain but they are not part of the resulting deformation,
/// i.e., points should not be mapped through these shifts. Therefore, we need
/// this deformation relation as a composition of other deformations while
/// ignoring certain parts of the composed deformation.
template <typename Surface>
class GenericRetriangulationDeformationRelation : public RetriangulationDeformationRelation<Surface> {
  using T = typename Surface::Coordinate;

 public:
  /// Create a deformation that relates `domain` and `codomain` by identifying
  /// the half edges `preimage` and `image`.
  GenericRetriangulationDeformationRelation(const Surface& domain, const Surface& codomain, HalfEdge preimage, HalfEdge image);

  /// Create a deformation that relates `domain` and `codomain` by identifying
  /// the straight paths `preimage` and `image`.
  GenericRetriangulationDeformationRelation(const Surface& domain, const Surface& codomain, Path<Surface> preimage, Path<Surface> image);

  std::optional<Path<Surface>> operator()(const Path<Surface>&) const override;
  Point<Surface> operator()(const Point<Surface>&) const override;

  std::optional<Path<Surface>> operator()(HalfEdge source, HalfEdge target, const Vector<T>& vector) const;

  /// Return a pair whose first entry is a saddle connection starting at
  /// `source` in `domain` and whose second entry is a corresponding saddle
  /// connection in `codomain`.
  /// The two entries might not be exactly equivalent if one of them stops at a
  /// marked point that does not exist in the other surface.
  std::optional<std::pair<SaddleConnection<Surface>, SaddleConnection<Surface>>> relation(const Vertex& source) const;

  std::optional<HalfEdge> source(HalfEdge source, const Vector<T>& vector) const;

  std::unique_ptr<DeformationRelation<Surface>> clone() const override;

  std::unique_ptr<DeformationRelation<Surface>> section() const override;

  static HalfEdge turn(const Surface&, HalfEdge source, Vector<T> direction, int angle);

  static HalfEdge turn(const Surface&, HalfEdge source, Vector<T> from, const Vector<T>& to);

  bool trivial() const override;

  std::ostream& operator>>(std::ostream&) const override;

  Path<Surface> preimage;
  Path<Surface> image;

 private:
  /// For each vertex in the domain, a pair of corresponding saddle connections, i.e., they are
  /// * starting at corresponding vertices
  /// * going in corresponding directions.
  /// However, they might not be equal, e.g., because one is shorter because it
  /// is hitting a marked point before it reaches the corresponding target
  /// vertex.
  mutable std::unordered_map<Vertex, std::pair<SaddleConnection<Surface>, SaddleConnection<Surface>>> relations;
};

}  // namespace flatsurf

#endif
