/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Julian Rüth
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

#include <vector>
#include <memory>
#include <ostream>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "external/rx-ranges/include/rx/ranges.hpp"

#include "../flatsurf/half_edge.hpp"

#include "impl/contour_decomposition.impl.hpp"

// TODO: We could add a lot of constness here probably.
// TODO: The "make()" from impl() pattern could probably be automated somewhat.
// TODO: const properties are almost always the wrong thing to do. Instead they
// should be private.

namespace flatsurf {

using std::vector;
using std::ostream;
  
template <typename Surface>
ContourDecomposition<Surface>::ContourDecomposition(std::unique_ptr<Surface> surface, const Vector<typename Surface::Coordinate>& vertical)
  : impl(spimpl::make_unique_impl<Implementation>(std::move(surface), vertical)) {}

template <typename Surface>
std::vector<ContourComponent<Surface>> ContourDecomposition<Surface>::components() const {
  vector<ContourComponent<Surface>> components;
  for (auto& component : impl->state->components)
    components.push_back(impl->state->make(&component));
  return components;
}

template <typename Surface>
std::shared_ptr<const typename Surface::Collapsed> ContourDecomposition<Surface>::collapsed() const {
  return impl->state->surface;
}

template <typename Surface>
Implementation<ContourDecomposition<Surface>>::Implementation(std::unique_ptr<Surface> surface, const Vector<typename Surface::Coordinate>& vertical) :
  state(new DecompositionState(std::move(surface), vertical)) {}

template <typename Surface>
ostream& operator<<(ostream& os, const ContourDecomposition<Surface>& self) {
  return os << fmt::format("[{}]", fmt::join(self.components() | rx::transform([&](const auto& component) { return fmt::format("{}", component); }) | rx::to_vector(), ", "));
}
}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), ContourDecomposition, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
