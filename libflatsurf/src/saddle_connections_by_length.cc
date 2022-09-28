/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020-2022 Julian Rüth
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
  self(spimpl::make_impl<ImplementationOf<SaddleConnectionsByLength>>(*connections.self)) {}

template <typename Surface>
typename SaddleConnectionsByLength<Surface>::iterator SaddleConnectionsByLength<Surface>::begin() const {
  auto ret = SaddleConnectionsByLengthIterator<Surface>(PrivateConstructor{}, *this);
  ret.self->increment();
  if (ret.self->connectionsWithinBounds.size())
    ret.self->currents.push_front(ret.self->connectionsWithinBounds.front());
  return ret;
}

template <typename Surface>
typename SaddleConnectionsByLength<Surface>::iterator SaddleConnectionsByLength<Surface>::end() const {
  return SaddleConnectionsByLengthIterator<Surface>(PrivateConstructor{}, *this);
}

template <typename Surface>
const Surface& SaddleConnectionsByLength<Surface>::surface() const {
  return self->surface;
}

template <typename Surface>
std::optional<Bound> SaddleConnectionsByLength<Surface>::bound() const {
  return self->searchRadius;
}

template <typename Surface>
std::optional<Bound> SaddleConnectionsByLength<Surface>::lowerBound() const {
  return byAngle().lowerBound();
}

template <typename Surface>
SaddleConnections<Surface> SaddleConnectionsByLength<Surface>::byAngle() const {
  SaddleConnections<Surface> connections(self->surface);
  *connections.self = *self;
  return connections;
}

template <typename Surface>
SaddleConnectionsByLength<Surface> SaddleConnectionsByLength<Surface>::bound(Bound bound) const {
  return this->byAngle().bound(bound).byLength();
}

template <typename Surface>
SaddleConnectionsByLength<Surface> SaddleConnectionsByLength<Surface>::lowerBound(Bound bound) const {
  return this->byAngle().lowerBound(bound).byLength();
}

template <typename Surface>
SaddleConnectionsByLength<Surface> SaddleConnectionsByLength<Surface>::sector(HalfEdge sectorBegin) const {
  return this->byAngle().sector(sectorBegin).byLength();
}

template <typename Surface>
SaddleConnectionsByLength<Surface> SaddleConnectionsByLength<Surface>::sector(const SaddleConnection<Surface>& sectorBegin, const SaddleConnection<Surface>& sectorEnd) const {
  return this->byAngle().sector(sectorBegin, sectorEnd).byLength();
}

template <typename Surface>
SaddleConnectionsByLength<Surface> SaddleConnectionsByLength<Surface>::sector(const Ray<Surface>& sectorBegin, const Ray<Surface>& sectorEnd) const {
  return this->byAngle().sector(sectorBegin, sectorEnd).byLength();
}

template <typename Surface>
SaddleConnectionsByLength<Surface> SaddleConnectionsByLength<Surface>::sector(const Vector<T>& sectorBegin, const Vector<T>& sectorEnd) const {
  return this->byAngle().sector(sectorBegin, sectorEnd).byLength();
}

template <typename Surface>
SaddleConnectionsByLength<Surface> SaddleConnectionsByLength<Surface>::source(const Vertex& source) const {
  return this->byAngle().source(source).byLength();
}

template <typename Surface>
ImplementationOf<SaddleConnectionsByLength<Surface>>::ImplementationOf(const ImplementationOf<SaddleConnections<Surface>>& connections) :
  ImplementationOf<SaddleConnections<Surface>>(connections) {}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const SaddleConnectionsByLength<Surface>&) {
  return os << "SaddleConnectionsByLength()";
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION)(LIBFLATSURF_INSTANTIATE_HASH), SaddleConnectionsByLength, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
