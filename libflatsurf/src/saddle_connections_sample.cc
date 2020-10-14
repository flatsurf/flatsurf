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

#include "../flatsurf/saddle_connections_sample.hpp"

#include <ostream>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/saddle_connections_sample_iterator.hpp"
#include "impl/saddle_connections_sample.impl.hpp"
#include "impl/saddle_connections_sample_iterator.impl.hpp"

namespace flatsurf {

template <typename Surface>
SaddleConnectionsSample<Surface>::SaddleConnectionsSample(const SaddleConnections<Surface>& connections) :
  self(spimpl::make_impl<ImplementationOf<SaddleConnectionsSample>>(*connections.self)) {}

template <typename Surface>
typename SaddleConnectionsSample<Surface>::iterator SaddleConnectionsSample<Surface>::begin() const {
  return SaddleConnectionsSampleIterator<Surface>(PrivateConstructor{}, *this);
}

template <typename Surface>
typename SaddleConnectionsSample<Surface>::iterator SaddleConnectionsSample<Surface>::end() const {
  // There is no end in random sampling. And since this is randomized, all iterators compare unequal so we can return anything as "end".
  return begin();
}

template <typename Surface>
const Surface& SaddleConnectionsSample<Surface>::surface() const {
  return self->surface;
}

template <typename Surface>
std::optional<Bound> SaddleConnectionsSample<Surface>::bound() const {
  return self->searchRadius;
}

template <typename Surface>
std::optional<Bound> SaddleConnectionsSample<Surface>::lowerBound() const {
  return byAngle().lowerBound();
}

template <typename Surface>
SaddleConnections<Surface> SaddleConnectionsSample<Surface>::byAngle() const {
  SaddleConnections<Surface> connections(self->surface);
  *connections.self = *self;
  return connections;
}

template <typename Surface>
SaddleConnectionsSample<Surface> SaddleConnectionsSample<Surface>::bound(Bound bound) const {
  return this->byAngle().bound(bound).sample();
}

template <typename Surface>
SaddleConnectionsSample<Surface> SaddleConnectionsSample<Surface>::lowerBound(Bound bound) const {
  return this->byAngle().lowerBound(bound).sample();
}

template <typename Surface>
SaddleConnectionsSample<Surface> SaddleConnectionsSample<Surface>::sector(HalfEdge sectorBegin) const {
  return this->byAngle().sector(sectorBegin).sample();
}

template <typename Surface>
SaddleConnectionsSample<Surface> SaddleConnectionsSample<Surface>::sector(const SaddleConnection<Surface>& sectorBegin, const SaddleConnection<Surface>& sectorEnd) const {
  return this->byAngle().sector(sectorBegin, sectorEnd).sample();
}

template <typename Surface>
SaddleConnectionsSample<Surface> SaddleConnectionsSample<Surface>::sector(const Vector<T>& sectorBegin, const Vector<T>& sectorEnd) const {
  return this->byAngle().sector(sectorBegin, sectorEnd).sample();
}

template <typename Surface>
SaddleConnectionsSample<Surface> SaddleConnectionsSample<Surface>::source(const Vertex& source) const {
  return this->byAngle().source(source).sample();
}

template <typename Surface>
ImplementationOf<SaddleConnectionsSample<Surface>>::ImplementationOf(const ImplementationOf<SaddleConnections<Surface>>& connections) :
  ImplementationOf<SaddleConnections<Surface>>(connections) {}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const SaddleConnectionsSample<Surface>&) {
  return os << "SaddleConnectionsSample()";
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION)(LIBFLATSURF_INSTANTIATE_HASH), SaddleConnectionsSample, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)

