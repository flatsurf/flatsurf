/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2020 Julian Rüth
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

#include "../flatsurf/saddle_connections.hpp"

#include <fmt/format.h>

#include <algorithm>
#include <exact-real/arb.hpp>
#include <stack>
#include <tuple>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/ccw.hpp"
#include "../flatsurf/chain.hpp"
#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/fmt.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/orientation.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections_by_length.hpp"
#include "../flatsurf/saddle_connections_iterator.hpp"
#include "../flatsurf/saddle_connections_sample.hpp"
#include "../flatsurf/vector.hpp"
#include "external/rx-ranges/include/rx/ranges.hpp"
#include "impl/saddle_connections.impl.hpp"
#include "impl/saddle_connections_by_length.impl.hpp"
#include "impl/saddle_connections_iterator.impl.hpp"
#include "util/assert.ipp"

namespace flatsurf {

template <typename Surface>
SaddleConnections<Surface>::SaddleConnections(const Surface& surface) :
  self(spimpl::make_impl<ImplementationOf<SaddleConnections>>(surface)) {}

template <typename Surface>
SaddleConnections<Surface> SaddleConnections<Surface>::bound(const Bound searchRadius) const {
  SaddleConnections<Surface> ret(self->surface);
  ret.self = self;
  ret.self->searchRadius = ret.self->searchRadius ? std::min(*ret.self->searchRadius, searchRadius) : searchRadius;
  return ret;
}

template <typename Surface>
std::optional<Bound> SaddleConnections<Surface>::lowerBound() const {
  if (self->lowerBound)
    return self->lowerBound;
  return std::nullopt;
}

template <typename Surface>
std::optional<Bound> SaddleConnections<Surface>::bound() const {
  return self->searchRadius;
}

template <typename Surface>
SaddleConnections<Surface> SaddleConnections<Surface>::lowerBound(const Bound bound) const {
  SaddleConnections<Surface> ret(self->surface);
  ret.self = self;
  ret.self->lowerBound = std::max(ret.self->lowerBound, bound);
  return ret;
}

template <typename Surface>
SaddleConnections<Surface> SaddleConnections<Surface>::source(const Vertex& source) const {
  SaddleConnections<Surface> ret(self->surface);
  ret.self = self;
  ret.self->sectors = ret.self->sectors | rx::filter([&](const auto& sector) {
    return Vertex::source(sector.source, *self->surface) == source;
  }) | rx::to_vector();
  return ret;
}

template <typename Surface>
SaddleConnections<Surface> SaddleConnections<Surface>::sector(const HalfEdge source) const {
  auto ret = *this;

  ret.self->sectors = ret.self->sectors | rx::filter([&](const auto& sector) {
    return sector.source == source;
  }) | rx::to_vector();
  return ret;
}

template <typename Surface>
SaddleConnections<Surface> SaddleConnections<Surface>::sector(const Vector<T>& sectorBegin, const Vector<T>& sectorEnd) const {
  auto ret = *this;

  std::vector<typename ImplementationOf<SaddleConnections>::Sector> sectors;
  for (const auto& sector : ret.self->sectors)
    for (const auto refined : sector.refine(surface(), sectorBegin, sectorEnd))
      sectors.push_back(refined);

  ret.self->sectors = sectors;

  return ret;
}

template <typename Surface>
SaddleConnections<Surface> SaddleConnections<Surface>::sector(const SaddleConnection<Surface>& sectorBegin, const SaddleConnection<Surface>& sectorEnd) const {
  auto ret = (*this)
                 .source(Vertex::source(sectorBegin.source(), surface()))
                 .source(Vertex::source(sectorEnd.source(), surface()));

  std::vector<typename ImplementationOf<SaddleConnections>::Sector> sectors;
  for (const auto& sector : ret.self->sectors) {
    if (sectorBegin.source() == sectorEnd.source()) {
      if (sector.source == sectorBegin.source())
        for (const auto& refined : sector.refine(surface(), sectorBegin, sectorEnd))
          sectors.push_back(refined);
      else {
        if (sectorBegin.vector().ccw(sectorEnd.vector()) == CCW::CLOCKWISE)
          sectors.push_back(sector);
      }
    } else {
      if (sector.source == sectorBegin.source()) {
        for (const auto& refined : sector.refine(surface(), sectorBegin, surface().fromHalfEdge(surface().nextAtVertex(sector.source))))
          sectors.push_back(refined);
      } else if (sector.source == sectorEnd.source()) {
        if (surface().fromHalfEdge(sector.source).ccw(sectorEnd) == CCW::COLLINEAR)
          // We must not call refine in this case as that method considers the
          // boundaries inclusive if they coincide. However, the end boundary
          // is exclusive here.
          ;
        else
          for (const auto& refined : sector.refine(surface(), surface().fromHalfEdge(sector.source), sectorEnd))
            sectors.push_back(refined);
      } else {
        for (HalfEdge walk = sector.source; walk != sectorBegin.source(); walk = surface().nextAtVertex(walk)) {
          if (walk == sectorEnd.source()) {
            sectors.push_back(sector);
            break;
          }
        }
      }
    }
  }

  ret.self->sectors = sectors;
  return ret;
}

template <typename Surface>
typename SaddleConnections<Surface>::iterator SaddleConnections<Surface>::begin() const {
  return SaddleConnectionsIterator<Surface>(PrivateConstructor{}, *self, cbegin(self->sectors), cend(self->sectors));
}

template <typename Surface>
typename SaddleConnections<Surface>::iterator SaddleConnections<Surface>::end() const {
  return SaddleConnectionsIterator<Surface>(PrivateConstructor{}, *self, cend(self->sectors), cend(self->sectors));
}

template <typename Surface>
SaddleConnectionsByLength<Surface> SaddleConnections<Surface>::byLength() const {
  return SaddleConnectionsByLength<Surface>(*this);
}

template <typename Surface>
SaddleConnectionsSample<Surface> SaddleConnections<Surface>::sample() const {
  return SaddleConnectionsSample<Surface>(*this);
}

template <typename Surface>
const Surface& SaddleConnections<Surface>::surface() const {
  return self->surface;
}

template <typename Surface>
ImplementationOf<SaddleConnections<Surface>>::ImplementationOf(const Surface& surface) :
  surface(surface),
  sectors(surface.halfEdges() | rx::transform([](const auto he) { return Sector(he); }) | rx::to_vector()) {}

template <typename Surface>
void ImplementationOf<SaddleConnections<Surface>>::resetLowerBound(SaddleConnections<Surface>& connections) {
  connections.self->lowerBound = 0;
}

template <typename Surface>
std::vector<typename ImplementationOf<SaddleConnections<Surface>>::Sector> ImplementationOf<SaddleConnections<Surface>>::Sector::refine(const Surface& surface, const Vector<T>& sectorBegin, const Vector<T>& sectorEnd) const {
  auto sector = this->sector ? *this->sector : std::pair{surface.fromHalfEdge(source), surface.fromHalfEdge(surface.nextAtVertex(source))};

  LIBFLATSURF_ASSERT(sector.first.ccw(surface.fromHalfEdge(source)) != CCW::CLOCKWISE, "sector boundaries before refinement must not be outside of search sector");
  LIBFLATSURF_ASSERT(sector.second.ccw(surface.fromHalfEdge(source)) != CCW::COUNTERCLOCKWISE, "sector boundaries before refinement must not be outside of search sector");

  const auto inSector = [](const auto& v, const auto& begin, const auto& end) {
    return v.inSector(begin, end);
  };

  const auto inSectorExclusive = [](const auto& v, const auto& begin, const auto& end) {
    return v.inSector(begin, end) && !(v.ccw(begin) == CCW::COLLINEAR && v.orientation(begin) == ORIENTATION::SAME);
  };

  if (inSector(sectorBegin, sector.first, sector.second)) {
    if (sectorBegin.ccw(sectorEnd) == CCW::CLOCKWISE) {
      const auto fromBegin = Sector(source, sectorBegin, sector.second);
      const auto toEnd = Sector(source, sector.first, sectorEnd);
      if (sector.first.ccw(sectorEnd) == CCW::COUNTERCLOCKWISE)
        return {fromBegin, toEnd};
      else
        return {fromBegin};
    } else if (inSector(sectorEnd, sector.first, sector.second)) {
      return {Sector(source, sectorBegin, sectorEnd)};
    } else {
      return {Sector(source, sectorBegin, sector.second)};
    }
  } else if (inSectorExclusive(sectorEnd, sector.first, sector.second)) {
    return {Sector(source, sector.first, sectorEnd)};
  } else if (inSector(sector.first, sectorBegin, sectorEnd)) {
    return {*this};
  } else {
    return {};
  }
}

template <typename Surface>
bool ImplementationOf<SaddleConnections<Surface>>::Sector::contains(const SaddleConnection<Surface>& connection) const {
  if (connection.source() != source)
    return false;
  if (!sector)
    return true;
  return connection.vector().inSector(sector->first, sector->second);
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const SaddleConnections<Surface>&) {
  return os << "SaddleConnections()";
}
}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION)(LIBFLATSURF_INSTANTIATE_HASH), SaddleConnections, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
