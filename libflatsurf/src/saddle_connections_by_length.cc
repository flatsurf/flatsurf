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

#include "../flatsurf/saddle_connections_by_length.hpp"

#include <ostream>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/saddle_connections_by_length_iterator.hpp"
#include "impl/saddle_connections_by_length.impl.hpp"
#include "impl/saddle_connections_by_length_iterator.impl.hpp"

namespace flatsurf {

template <typename Surface>
SaddleConnectionsByLength<Surface>::SaddleConnectionsByLength(const SaddleConnections<Surface>& connections) :
  impl(spimpl::make_impl<Implementation>(*connections.impl)) {}

template <typename Surface>
typename SaddleConnectionsByLength<Surface>::Iterator SaddleConnectionsByLength<Surface>::begin() const {
  auto ret = SaddleConnectionsByLengthIterator<Surface>(PrivateConstructor{}, *this);
  ret.impl->increment();
  return ret;
}

template <typename Surface>
typename SaddleConnectionsByLength<Surface>::Iterator SaddleConnectionsByLength<Surface>::end() const {
  return SaddleConnectionsByLengthIterator<Surface>(PrivateConstructor{}, *this);
}

template <typename Surface>
const Surface& SaddleConnectionsByLength<Surface>::surface() const {
  return *impl->surface;
}

template <typename Surface>
std::optional<Bound> SaddleConnectionsByLength<Surface>::bound() const {
  return impl->searchRadius;
}

template <typename Surface>
SaddleConnections<Surface> SaddleConnectionsByLength<Surface>::byAngle() const {
  SaddleConnections<Surface> connections(impl->surface);
  *connections.impl = *impl;
  return connections;
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const SaddleConnectionsByLength<Surface>&) {
  return os << "SaddleConnectionsByLength()";
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION)(LIBFLATSURF_INSTANTIATE_HASH), SaddleConnectionsByLength, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
