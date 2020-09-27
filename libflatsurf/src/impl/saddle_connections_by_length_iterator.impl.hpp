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

#ifndef LIBFLATSURF_SADDLE_CONNECTIONS_BY_LENGTH_ITERATOR_IMPL_HPP
#define LIBFLATSURF_SADDLE_CONNECTIONS_BY_LENGTH_ITERATOR_IMPL_HPP

#include <deque>
#include <list>

#include "../../flatsurf/bound.hpp"
#include "../../flatsurf/saddle_connection.hpp"
#include "../../flatsurf/saddle_connections_by_length_iterator.hpp"

namespace flatsurf {

template <typename Surface>
class ImplementationOf<SaddleConnectionsByLengthIterator<Surface>> {
  using T = typename Surface::Coordinate;

 public:
  ImplementationOf(const SaddleConnectionsByLength<Surface>&);

  // Increment lowerBoundInclusive and upperBoundExclusive and fill
  // connectionsWithinBounds with the connections between [lowerBoundInclusive,
  // upperBoundExclusive) sorted by length. Keep increasing the bounds until
  // connectionsWithinBounds is non-empty. When during this process
  // lowerBoundInclusive exceeds the set bound, set lowerBoundInclusive =
  // upperBoundExclusive = 0 to mark that this is now the end() iterator.
  void increment();

  const SaddleConnectionsByLength<Surface>& connections;
  Bound lowerBoundExclusive;
  Bound upperBoundInclusive;
  std::deque<SaddleConnection<Surface>> connectionsWithinBounds;

  // This is a hack to work around https://bitbucket.org/wlav/cppyy/issues/271/next-implementation-does-not-respect.
  mutable std::list<SaddleConnection<Surface>> currents;
};

template <typename Surface>
template <typename... Args>
SaddleConnectionsByLengthIterator<Surface>::SaddleConnectionsByLengthIterator(PrivateConstructor, Args&&... args) :
  self(spimpl::make_impl<ImplementationOf<SaddleConnectionsByLengthIterator>>(std::forward<Args>(args)...)) {}

}  // namespace flatsurf

#endif
