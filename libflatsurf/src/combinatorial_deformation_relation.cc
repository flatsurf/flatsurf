/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2022 Julian Rüth
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

#include "../flatsurf/vector.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/point.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/permutation.hpp"
#include "../flatsurf/chain.hpp"
#include "../flatsurf/saddle_connection.hpp"
 
#include "impl/combinatorial_deformation_relation.hpp"

#include "util/assert.ipp"

namespace flatsurf {

template <typename Surface>
CombinatorialDeformationRelation<Surface>::CombinatorialDeformationRelation(const Surface& domain, const Surface& codomain, Permutation<HalfEdge> mapping) :
  DeformationRelation<Surface>(domain, codomain),
  mapping(std::move(mapping)),
  relabeling(true) {
  LIBFLATSURF_ASSERT(2*domain.size() == this->mapping.size(), "half edge mapping " << this->mapping << " is not compatible with surface " << domain);

  for (const Edge edge : domain.edges()) {
    const auto preimage = edge.positive();
    const auto image = this->mapping(preimage);

    LIBFLATSURF_ASSERT(this->mapping(-preimage) == -image, "mapping must preserve edges but " << this->mapping << " does not.");

    if (domain.fromHalfEdge(preimage) != codomain.fromHalfEdge(image)) {
      relabeling = false;
      break;
    }
  }
}

template <typename Surface>
std::optional<Path<Surface>> CombinatorialDeformationRelation<Surface>::operator()(const Path<Surface>& path) const {
  if (!relabeling)
    throw std::logic_error("not implemented: mapping paths through combinatorial non-trivial deformations");

  std::vector<SaddleConnection<Surface>> segments = path;

  for (auto& segment : segments) {
    Chain<Surface> chain(this->codomain);

    for (const auto& summand : segment.chain())
      chain += *summand.second * Chain<Surface>(this->codomain, this->mapping(summand.first.positive()));

    segment = SaddleConnection(*this->codomain, this->mapping(segment.source()), this->mapping(segment.target()), chain);
  }

  return Path(segments);
}

template <typename Surface>
Point<Surface> CombinatorialDeformationRelation<Surface>::operator()(const Point<Surface>& point) const {
  // This does not make a ton of sense but we just map points preserving their
  // barycentric coordinates.
  HalfEdge face = point.face();
  HalfEdge image = mapping(face);

  const auto coordinates = point.coordinates(face);
  return Point(*this->codomain, image, coordinates[0], coordinates[1], coordinates[2]);
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> CombinatorialDeformationRelation<Surface>::clone() const {
  return std::make_unique<CombinatorialDeformationRelation>(*this);
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> CombinatorialDeformationRelation<Surface>::section() const {
  throw std::logic_error("not implemented: section of combinatorial deformation");
}

template <typename Surface>
bool CombinatorialDeformationRelation<Surface>::trivial() const {
  if (!relabeling)
    return false;

  for (const Edge edge : this->domain->edges())
    if (mapping(edge.positive()) != edge.positive())
      return false;

  return true;
}

template <typename Surface>
std::ostream& CombinatorialDeformationRelation<Surface>::operator>>(std::ostream& os) const {
  return os << this->domain << " → " << this->codomain << " mapping half edges " << this->mapping;
}

}

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_STATIC), CombinatorialDeformationRelation, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
