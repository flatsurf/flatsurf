/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020 Julian Rüth
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

#include "../flatsurf/half_edge.hpp"
#include "impl/deformation.impl.hpp"

namespace flatsurf {

template <typename Surface>
Deformation<Surface>::Deformation(Surface&& surface) :
  self(spimpl::make_unique_impl<ImplementationOf<Deformation>>(std::move(surface))) {}

template <typename Surface>
Surface Deformation<Surface>::surface() {
  return self->surface.clone();
}

template <typename Surface>
const Surface& Deformation<Surface>::surface() const {
  return self->surface;
}

template <typename Surface>
std::optional<HalfEdge> Deformation<Surface>::operator()(HalfEdge he) const {
  return (*self)(he);
}

template <typename Surface>
ImplementationOf<Deformation<Surface>>::ImplementationOf(Surface&& surface) :
  surface(std::move(surface)) {}

template <typename Surface>
Deformation<Surface> ImplementationOf<Deformation<Surface>>::make(Surface&& surface) {
  return make(spimpl::make_unique_impl<ImplementationOf>(std::move(surface)));
}

template <typename Surface>
std::optional<HalfEdge> ImplementationOf<Deformation<Surface>>::operator()(HalfEdge) const {
  throw std::logic_error("not implemented: this deformation does not track mapping of half edges");
}

template <typename Surface>
Deformation<Surface> ImplementationOf<Deformation<Surface>>::make(spimpl::unique_impl_ptr<ImplementationOf>&& impl) {
  return Deformation<Surface>(PrivateConstructor{}, std::move(impl));
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const Deformation<Surface>& self) {
  return os << *self.self;
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const ImplementationOf<Deformation<Surface>>& self) {
  return os << self.surface;
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), Deformation, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
