/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2021 Julian Rüth
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

#include <iostream> // TODO

#include <fmt/format.h>

#include "../flatsurf/fmt.hpp"
#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/path_iterator.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/orientation.hpp"
#include "../flatsurf/vertex.hpp"
#include "../flatsurf/vertical.hpp"
#include "../flatsurf/ccw.hpp"
#include "impl/generic_retriangulation_deformation_relation.hpp"
#include "util/assert.ipp"

namespace flatsurf {

template <typename Surface>
GenericRetriangulationDeformationRelation<Surface>::GenericRetriangulationDeformationRelation(const Surface& domain, const Surface& codomain, std::vector<std::pair<Path<Surface>, Path<Surface>>> relation) : RetriangulationDeformationRelation<Surface>(domain, codomain), relation(std::move(relation)) {
  // TODO: Check arguments. Here and in all the other relations.
}

template <typename Surface>
std::optional<Path<Surface>> GenericRetriangulationDeformationRelation<Surface>::operator()(const Path<Surface>& path) const {
  if (path.size() == 0)
    return Path<Surface>{};

  for (auto& rel : relation) {
    if (rel.first == path)
      return rel.second;
    if (rel.first == -path)
      return -rel.second;
  }

  // TODO: What are the assumptions for this to be correct? We use this for operator+ where it is not correct.
  // TODO: Should we tighten first?
  Path<Surface> pending = path;

  HalfEdge source = pending.begin()->source();
  auto vector = pending.begin()->vector();

  pending.pop_front();

  while (!pending.empty()) {
    auto vector_ = pending.begin()->vector();
    if (vector.ccw(vector_) == CCW::COLLINEAR && vector.orientation(vector_) == ORIENTATION::SAME && this->domain->angle(Vertex::source(pending.begin()->source(), *this->domain)) == 1) {
      // The first entries of the path are collinear and cross over a marked
      // point. Combine them into one (search) vector.
      vector += vector_;
      pending.pop_front();
    }
  }

  auto source_ = [&]() {
    for (auto& rel : relation) {
      if (rel.first == SaddleConnection<Surface>{this->domain, source})
        return rel.second;
      if (rel.first == -SaddleConnection<Surface>{this->domain, source})
        return -rel.second;
    }
    throw std::logic_error("not implemented: cannot determine image when relation is not given on the level of half edges yet.");
  }();

  if (source_.size() == 0) {
    std::cout << "Cannot find source " << source << std::endl;
    // TODO: When is this allowed to happen?
    return std::nullopt;
  }

  if (source_.size() > 1) {
    // TODO: Assert that the bits are collinear.
  }

  Path<Surface> path_;
  Vector<typename Surface::Coordinate> vector_;

  const auto extend = [&](auto&& connection) {
    vector_ += connection.vector();
    path_.push_back(connection);
  };

  extend(SaddleConnection<Surface>::counterclockwise(this->codomain, *source_.begin(), Vertical<Surface>(this->codomain, vector)));

  while (vector != vector_) {
    LIBFLATSURF_ASSERT((vector - vector_).orientation(vector) == ORIENTATION::SAME, "Partial path " << path_ << " was longer than " << path << " but it can only be shorter.");

    HalfEdge target = (-path_).begin()->source();

    LIBFLATSURF_ASSERT(this->codomain->angle(Vertex::source(target, *this->codomain)) == 1, "Image of saddle connection crosses over non-marked point.");

    extend(SaddleConnection<Surface>::inPlane(this->codomain, this->codomain->nextAtVertex(target), vector - vector_));
  }

  auto pending_ = this->operator()(pending);

  if (!pending_)
    return std::nullopt;

  return path_ + *pending_;
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> GenericRetriangulationDeformationRelation<Surface>::clone() const {
  return std::make_unique<GenericRetriangulationDeformationRelation<Surface>>(*this);
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> GenericRetriangulationDeformationRelation<Surface>::section() const {
  auto reversed = relation;
  for (auto& rel : reversed)
    std::swap(rel.first, rel.second);
  return std::make_unique<GenericRetriangulationDeformationRelation>(this->codomain, this->domain, reversed);
}

template <typename Surface>
bool GenericRetriangulationDeformationRelation<Surface>::trivial() const {
  if (this->domain != this->codomain)
    return false;

  for (auto rel : relation)
    if (rel.first != rel.second)
      return false;

  return true;
}

template <typename Surface>
std::ostream& GenericRetriangulationDeformationRelation<Surface>::operator>>(std::ostream& os) const {
  return os << this->domain << " → " << this->codomain << " given by " << fmt::format("{{{}}}", fmt::join([&]() {
    std::vector<std::string> relations;
    for (const auto& rel : relation)
      relations.push_back(fmt::format("{} ≈ {}", rel.first, rel.second));
    return relations;
  }(), ", "));
}

}

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_STATIC), GenericRetriangulationDeformationRelation, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
