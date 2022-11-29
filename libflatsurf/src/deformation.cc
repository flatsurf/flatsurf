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

#include <ostream>
#include <stdexcept>

#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/point.hpp"
#include "../flatsurf/ray.hpp"
#include "../flatsurf/path_iterator.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "impl/composite_deformation_relation.hpp"
#include "impl/deformation.impl.hpp"
#include "impl/trivial_deformation_relation.hpp"
#include "util/assert.ipp"

namespace flatsurf {

template <typename Surface>
Deformation<Surface>::Deformation(const Deformation& deformation) :
  Deformation(PrivateConstructor{}, deformation.self->relation->clone()) {}

template <typename Surface>
Deformation<Surface>::Deformation(Deformation&& deformation) :
  Deformation(PrivateConstructor{}, std::move(deformation.self->relation)) {}

template <typename Surface>
Deformation<Surface>::Deformation(Surface&& surface) :
  Deformation(surface) {}

template <typename Surface>
Deformation<Surface>::Deformation(const Surface& surface) :
  Deformation(PrivateConstructor{}, std::make_unique<TrivialDeformationRelation<Surface>>(surface)) {}

template <typename Surface>
Surface Deformation<Surface>::surface() {
  return this->codomain().clone();
}

template <typename Surface>
const Surface& Deformation<Surface>::surface() const {
  return this->codomain();
}

template <typename Surface>
const Surface& Deformation<Surface>::domain() const {
  return self->relation->domain;
}

template <typename Surface>
const Surface& Deformation<Surface>::codomain() const {
  return self->relation->codomain;
}

template <typename Surface>
Deformation<Surface>& Deformation<Surface>::operator=(const Deformation& deformation) {
  self->relation = deformation.self->relation->clone();
  return *this;
}

template <typename Surface>
Deformation<Surface>& Deformation<Surface>::operator=(Deformation&& deformation) {
  self->relation = std::move(deformation.self->relation);
  return *this;
}

template <typename Surface>
std::optional<HalfEdge> Deformation<Surface>::operator()(HalfEdge he) const {
  auto image = operator()(SaddleConnection<Surface>(domain(), he));
  if (!image)
    return std::nullopt;
  if (image->size() == 1)
    if (*image->begin() == SaddleConnection<Surface>(codomain(), (*image->begin()).source()))
      return (*image->begin()).source();
  throw std::invalid_argument("Half edge does not map to a single half edge under this deformation.");
}

template <typename Surface>
std::optional<Path<Surface>> Deformation<Surface>::operator()(const Path<Surface>& path) const {
  return self->relation->operator()(path);
}

template <typename Surface>
Point<Surface> Deformation<Surface>::operator()(const Point<Surface>& point) const {
  LIBFLATSURF_CHECK_ARGUMENT(point.surface() == domain(), "point must be in the domain of the deformation");
  return self->relation->operator()(point);
}

template <typename Surface>
Ray<Surface> Deformation<Surface>::operator()(const Ray<Surface>& ray) const {
  LIBFLATSURF_CHECK_ARGUMENT(ray.surface() == domain(), "ray must be in the domain of the deformation");
  return self->relation->operator()(ray);
}

template <typename Surface>
Deformation<Surface> Deformation<Surface>::operator*(const Deformation<Surface>& rhs) const {
  LIBFLATSURF_CHECK_ARGUMENT(rhs.codomain() == domain(), "Deformations are not compatible. Cannot compose " << *this << " and " << rhs << " since the codomain of the latter does not match the domain of the former.");

  if (this->trivial())
    return ImplementationOf<Deformation>::make(rhs.self->relation->clone());
  if (rhs.trivial())
    return ImplementationOf<Deformation>::make(self->relation->clone());

  return ImplementationOf<Deformation>::make(std::make_unique<CompositeDeformationRelation<Surface>>(*self->relation, *rhs.self->relation));
}

template <typename Surface>
Deformation<Surface> Deformation<Surface>::section() const {
  return ImplementationOf<Deformation>::make(self->relation->section());
}

template <typename Surface>
bool Deformation<Surface>::trivial() const {
  LIBFLATSURF_ASSERT(self->relation, "Deformation not in a valid state. Has it been moved out?");

  if (self->relation->trivial()) {
    if (dynamic_cast<TrivialDeformationRelation<Surface>*>(self->relation.get()) == nullptr)
      self->relation = std::make_unique<TrivialDeformationRelation<Surface>>(domain());
    return true;
  }

  return false;
}

template <typename Surface>
ImplementationOf<Deformation<Surface>>::ImplementationOf(std::unique_ptr<DeformationRelation<Surface>> relation) :
  relation(std::move(relation)) {}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const Deformation<Surface>& self) {
  return os << *self.self->relation;
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), Deformation, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
