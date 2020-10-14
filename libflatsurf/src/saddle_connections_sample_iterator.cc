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

#include <random>

#include "../flatsurf/ccw.hpp"
#include "../flatsurf/saddle_connections_iterator.hpp"
#include "../flatsurf/saddle_connections_sample.hpp"
#include "impl/saddle_connections_sample.impl.hpp"
#include "impl/saddle_connections_sample_iterator.impl.hpp"

namespace flatsurf {

template <typename Surface>
void SaddleConnectionsSampleIterator<Surface>::increment() {
  if (self->connections.bound())
    throw std::logic_error("not implemented: random sampling of saddle connections is not implemented when there is an upper bound set");

  const auto& sectors = self->connections.self->sectors;

  if (sectors.size() == 0)
    throw std::logic_error("not implemented: random sampling of empty sets of saddle connections is not implemented");

  std::uniform_int_distribution<> sector_distribution(0, static_cast<int>(sectors.size()) - 1);
  auto& sector = sectors[sector_distribution(self->rand)];

  auto connections = self->connections.byAngle().sector(sector.source);
  ASSERT(!sector.sector || sector.sector->first != sector.sector->second, "SaddleConnections contains an empty sector, such sectors should have been thrown out from the list of sectors");

  const auto sectorBegin = sector.sector ? sector.sector->first : self->connections.surface().fromHalfEdge(sector.source);
  const auto sectorEnd = sector.sector ? sector.sector->second : self->connections.surface().fromHalfEdge(self->connections.surface().nextAtVertex(sector.source));

  using std::begin;
  using std::end;

  std::uniform_int_distribution<> ccw_distribution(0, 1);

  auto it = begin(connections);
  it++;
  while (true) {
    self->current = *it;

    if (self->seen.find(self->current) != end(self->seen)) {
      if (self->current.vector() == sectorBegin)
        it.skipSector(CCW::CLOCKWISE);
      else if (self->current.vector() == sectorEnd)
        it.skipSector(CCW::COUNTERCLOCKWISE);
      else if (ccw_distribution(self->rand) % 2)
        it.skipSector(CCW::CLOCKWISE);
      else
        it.skipSector(CCW::COUNTERCLOCKWISE);

      ++it;
      continue;
    }

    self->seen.insert(self->current);
    return;
  }
}

template <typename Surface>
const SaddleConnection<Surface>& SaddleConnectionsSampleIterator<Surface>::dereference() const {
  return self->current;
}

template <typename Surface>
bool SaddleConnectionsSampleIterator<Surface>::equal(const SaddleConnectionsSampleIterator& rhs) const {
  // Random iterators never terminate and due to their randomness are incomparable.
  return this == &rhs;
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const SaddleConnectionsSampleIterator<Surface>&) {
  return os << "SaddleConnectionsSampleIterator()";
}

template <typename Surface>
ImplementationOf<SaddleConnectionsSampleIterator<Surface>>::ImplementationOf(const SaddleConnectionsSample<Surface>& connections) :
  connections(connections),
  seen(),
  current(connections.surface(), connections.self->sectors[0].source),
  rand(std::random_device()()) {}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION)(LIBFLATSURF_INSTANTIATE_HASH), SaddleConnectionsSampleIterator, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
