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

#include <fmt/format.h>
#include <fmt/ranges.h>

#include "../flatsurf/vector.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/point.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/permutation.hpp"
#include "../flatsurf/chain.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/fmt.hpp"
 
#include "impl/combinatorial_deformation_relation.hpp"

#include "util/assert.ipp"

namespace flatsurf {

namespace {

std::string formatMapping(const std::unordered_map<HalfEdge, HalfEdge>& mapping) {
  return fmt::format("{{{}}}", fmt::join(mapping, ", "));
}

}

template <typename Surface>
CombinatorialDeformationRelation<Surface>::CombinatorialDeformationRelation(const Surface& domain, const Surface& codomain, std::unordered_map<HalfEdge, HalfEdge> mapping) :
  DeformationRelation<Surface>(domain, codomain),
  mapping(std::move(mapping)),
  relabeling(true) {
  LIBFLATSURF_ASSERT(2*domain.size() == this->mapping.size(), "half edge mapping " << formatMapping(this->mapping) << " is not compatible with surface " << domain);

  for (const Edge edge : domain.edges()) {
    const auto preimage = edge.positive();
    const auto image = this->mapping.find(preimage);
    const auto minus_image = this->mapping.find(-preimage);

    LIBFLATSURF_ASSERT((image == std::end(this->mapping)) == (minus_image == std::end(this->mapping)), "Combinatorial equivalence must be defined on edges but it maps " << preimage << " but does not map " << -preimage);

    if (image == std::end(this->mapping)) {
      relabeling = false;
      continue;
    }


    LIBFLATSURF_ASSERT(minus_image->second == -image->second, "mapping must preserve edges but " << formatMapping(this->mapping) << " does not.");

    if (domain.fromHalfEdge(preimage) != codomain.fromHalfEdge(image->second)) {
      relabeling = false;
      break;
    }
  }
}

template <typename Surface>
CombinatorialDeformationRelation<Surface>::CombinatorialDeformationRelation(const Surface& domain, const Surface& codomain, const Permutation<HalfEdge>& mapping) :
  CombinatorialDeformationRelation(domain, codomain, [&](){
    std::unordered_map<HalfEdge, HalfEdge> map;

    for (const HalfEdge preimage : mapping.domain())
      map[preimage] = mapping(preimage);

    return map;
  }()) {
}

template <typename Surface>
std::optional<Path<Surface>> CombinatorialDeformationRelation<Surface>::operator()(const Path<Surface>& path) const {
  if (!relabeling)
    throw std::logic_error("not implemented: mapping paths through combinatorial non-trivial deformations");

  std::vector<SaddleConnection<Surface>> segments = path;

  for (auto& segment : segments) {
    Chain<Surface> chain(this->codomain);

    for (const auto& summand : segment.chain())
      chain += *summand.second * Chain<Surface>(this->codomain, this->mapping.find(summand.first.positive())->second);

    segment = SaddleConnection(*this->codomain, this->mapping.find(segment.source())->second, this->mapping.find(segment.target())->second, chain);
  }

  return Path(segments);
}

template <typename Surface>
Point<Surface> CombinatorialDeformationRelation<Surface>::operator()(const Point<Surface>& point) const {
  if (!relabeling)
    throw std::logic_error("not implemented: mapping points through combinatorial non-trivial deformations");

  HalfEdge face = point.face();
  HalfEdge image = mapping.find(face)->second;

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
    if (mapping.find(edge.positive())->second != edge.positive())
      return false;

  return true;
}

template <typename Surface>
std::ostream& CombinatorialDeformationRelation<Surface>::operator>>(std::ostream& os) const {
  return os << this->domain << " → " << this->codomain << " mapping half edges " << formatMapping(this->mapping);
}

}

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_STATIC), CombinatorialDeformationRelation, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
