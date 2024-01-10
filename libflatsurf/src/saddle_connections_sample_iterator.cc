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

#include <cmath>
#include <ostream>
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
  auto connections = this->connections.byAngle().sector(sector.source);
  ImplementationOf<SaddleConnections<Surface>>::resetLowerBound(connections);

  LIBFLATSURF_ASSERT(!sector.sector || sector.sector->first != sector.sector->second, "SaddleConnections contains an empty sector, such sectors should have been thrown out from the list of sectors");

  const auto outerSectorBegin = sector.sector ? sector.sector->first : connections.surface().fromHalfEdge(sector.source);
  const auto outerSectorEnd = sector.sector ? sector.sector->second : connections.surface().fromHalfEdge(connections.surface().nextAtVertex(sector.source));

  using std::begin;
  using std::end;

  auto sectorBegin = outerSectorBegin;
  auto sectorEnd = outerSectorEnd;

  const auto dbl = [](const auto& x) {
    if constexpr (std::is_same_v<std::decay_t<decltype(x)>, mpq_class> || std::is_same_v<std::decay_t<decltype(x)>, mpz_class>) {
      return x.get_d();
    } else {
      return static_cast<double>(x);
    }
  };

  const auto ccw = [&]() {
    // When we see a saddle connection that we had already seen before, (or that
    // is too short) we need to decide on which side of that connection we want
    // to continue our search. Just picking the sides with equal probability
    // leads to very small sectors quickly. Instead, we want to weigh the random
    // choice by the size of the corresponding search sector.

    const auto v = sectorBegin;
    const auto w = sectorEnd;
    const auto c = current.vector();

    LIBFLATSURF_ASSERT(c.inSector(v, w), "Connection must be contained in the bounding sector but " << c << " is not in the sector spanned by " << v << " and " << w);

    // We want the probability to go to the clockwise side to be p = α / β
    // where α is the angle (v, c) and β the angle (v, w), i.e., the further c
    // is from v the more likely we go to the clockwise side.
    // Since all angles are typically extremely small, we can assume that
    // sin(x) = x. When the above probability translates to p = v×c / v×w ·
    // |w|/|c|.
    // Computing these fractions is expensive or actually not possible for some
    // types T.  If we cast everything to double, then v×c == v×w which does
    // not help either. So instead, we estimate everything in a very crude way:
    // The quotient |w|/|c| can safely be represented with double precision.
    double wc = sqrt(dbl(T(w * w)) / dbl(T(c * c)));

    if (!std::isfinite(wc))
      throw std::logic_error("not implemented: saddle connection went into a sector with too distinct boundaries");

    // The cross products v×c and v×w are often virtually indistinguishable
    // when written in double precision. Therefore, we successively estimate
    // their quotient in a very crude way.
    T vc = v.x() * c.y() - v.y() * c.x();
    T vw = v.x() * w.y() - v.y() * w.x();

    if (wc < 1)
      vw *= static_cast<int>(1 / wc);
    if (wc > 1)
      vc *= static_cast<int>(wc);

    while (true) {
      if (2 * vc < vw) {
        if (std::uniform_real_distribution<>(0, 1)(rand) >= .5)
          return CCW::COUNTERCLOCKWISE;
        vc *= 2;
      } else {
        if (std::uniform_real_distribution<>(0, 1)(rand) < .5)
          return CCW::CLOCKWISE;
        vw *= 2;
      }
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
