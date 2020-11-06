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
  self->increment();
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
  rand(std::random_device()()) {
  increment();
}

template <typename Surface>
void ImplementationOf<SaddleConnectionsSampleIterator<Surface>>::increment() {
  if (connections.bound())
    throw std::logic_error("not implemented: random sampling of saddle connections is not implemented when there is an upper bound set");

  const auto& sectors = connections.self->sectors;

  if (sectors.size() == 0)
    throw std::logic_error("not implemented: random sampling of empty sets of saddle connections is not implemented");

  std::uniform_int_distribution<> sector_distribution(0, static_cast<int>(sectors.size()) - 1);
  auto& sector = sectors[sector_distribution(rand)];

  // We rely on iteration through the saddle connections "by angle". However,
  // we completely override the original logic there since we reset the
  // lowerBound and call skipSector in every iteration.
  auto connections = this->connections.byAngle().lowerBound(0).sector(sector.source);
  ASSERT(!sector.sector || sector.sector->first != sector.sector->second, "SaddleConnections contains an empty sector, such sectors should have been thrown out from the list of sectors");

  const auto outerSectorBegin = sector.sector ? sector.sector->first : connections.surface().fromHalfEdge(sector.source);
  const auto outerSectorEnd = sector.sector ? sector.sector->second : connections.surface().fromHalfEdge(connections.surface().nextAtVertex(sector.source));

  using std::begin;
  using std::end;

  auto sectorBegin = outerSectorBegin;
  auto sectorEnd = outerSectorEnd;

  const auto ccw = [&]() {
    // When we see a saddle connection that we had already seen before, (or that
    // is too short) we need to decide on which side of that connection we want
    // to continue our search. Just picking the sides with equal probability
    // leads to very small sectors quickly. Instead, we want to weigh the random
    // choice by the size of the corresponding search sector.

    const auto angle = [](const auto& x, const auto& y) {
      // Ideally, we would compute the angles enclosed between (sectorBegin,
      // connection) and (connection, sectorEnd) and then give probabilities
      // relative to their sizes. Exact computations are way too expensive here
      // so we only use double precision and hope for the best. To help
      // stability a bit, we do not use standard angles for the Euclidean norm
      // but rather what's sometimes called diamond angles, i.e., essentially
      // an angle in the 1-norm. These angles are in [0, 4) instead of [0, 2π),
      // see e.g.
      // https://www.freesteel.co.uk/wpblog/2009/06/05/encoding-2d-angles-without-trigonometry/
      // The distribution of such angles is not the same of course but it should be
      // good enough for our purposes.
      double xx, yy;
      if constexpr (std::is_same_v<T, mpq_class> || std::is_same_v<T, mpz_class>) {
        xx = x.get_d();
        yy = x.get_d();
      } else {
        xx = static_cast<double>(x);
        yy = static_cast<double>(y);
      }

      if (yy >= 0) {
        if (xx >= 0)
          return 0 + yy / (xx + yy);
        else
          return 1 + (-xx) / (yy + (-xx));
      } else {
        if (xx < 0)
          return 2 + (-yy) / ((-xx) + (-yy));
        else
          return 3 + xx / (xx + (-yy));
      }
    };

    const double sectorBeginAngle = angle(sectorBegin.x(), sectorBegin.y());
    const double sectorEndAngle = angle(sectorEnd.x(), sectorEnd.y());
    const double connectionAngle = angle(current.vector().x(), current.vector().y());

    double totalAngle = sectorEndAngle - sectorBeginAngle;
    if (totalAngle < 0) totalAngle += 4;
    double clockwiseAngle = connectionAngle - sectorBeginAngle;
    if (clockwiseAngle < 0) clockwiseAngle += 4;

    if (isfinite(totalAngle) && isfinite(clockwiseAngle)) {
      return std::uniform_real_distribution<>(0, totalAngle)(rand) < clockwiseAngle ?
        CCW::CLOCKWISE : CCW::COUNTERCLOCKWISE;
    } else {
      // The computation was unstable and produced infinities or NaN. That
      // might happen when the double values get extremely small. Just pick a
      // side randomly.
      return std::uniform_int_distribution<>(0, 1)(rand) == 0 ?
        CCW::CLOCKWISE : CCW::COUNTERCLOCKWISE;
    }
  };

  auto it = begin(connections);
  it++;
  while (true) {
    current = *it;

    bool eligible = current > this->connections.lowerBound();

    if (eligible) {
      if (seen.find(current) == end(seen))
        seen.insert(current);
      else
        eligible = false;
    }

    if (!eligible) {
      if (current.vector() == outerSectorBegin)
        it.skipSector(CCW::CLOCKWISE);
      else if (current.vector() == outerSectorEnd)
        it.skipSector(CCW::COUNTERCLOCKWISE);
      else if (ccw() == CCW::COUNTERCLOCKWISE) {
        it.skipSector(CCW::CLOCKWISE);
        sectorBegin = current;
      } else {
        it.skipSector(CCW::COUNTERCLOCKWISE);
        sectorEnd = current;
      }

      ++it;
      continue;
    }

    return;
  }
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION)(LIBFLATSURF_INSTANTIATE_HASH), SaddleConnectionsSampleIterator, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
