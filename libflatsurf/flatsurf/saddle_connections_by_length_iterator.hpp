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

#ifndef LIBFLATSURF_SADDLE_CONNECTIONS_BY_LENGTH_ITERATOR_HPP
#define LIBFLATSURF_SADDLE_CONNECTIONS_BY_LENGTH_ITERATOR_HPP

#include <boost/iterator/iterator_facade.hpp>
#include <iosfwd>

#include "copyable.hpp"

namespace flatsurf {

// Iterates over the saddle connections on a triangulation translation surface in increasing length.
template <typename Surface>
class SaddleConnectionsByLengthIterator : public boost::iterator_facade<SaddleConnectionsByLengthIterator<Surface>, const SaddleConnection<Surface>, boost::forward_traversal_tag> {
  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type parameter must not have modifiers such as const");

  using T = typename Surface::Coordinate;

  template <typename... Args>
  SaddleConnectionsByLengthIterator(PrivateConstructor, Args &&... args);

 public:
  // Advance the iterator to the next saddle connection.
  void increment();
  bool equal(const SaddleConnectionsByLengthIterator &other) const;
  const SaddleConnection<Surface> &dereference() const;

  template <typename S>
  friend std::ostream &operator<<(std::ostream &, const SaddleConnectionsByLengthIterator<S> &);

 private:
  Copyable<SaddleConnectionsByLengthIterator> self;

  friend SaddleConnectionsByLength<Surface>;
  friend ImplementationOf<SaddleConnectionsByLengthIterator>;
};

}  // namespace flatsurf

#endif
