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

#ifndef LIBFLATSURF_SADDLE_CONNECTIONS_SAMPLE_ITERATOR_HPP
#define LIBFLATSURF_SADDLE_CONNECTIONS_SAMPLE_ITERATOR_HPP

#include <iosfwd>
#include <boost/iterator/iterator_facade.hpp>

#include "copyable.hpp"

namespace flatsurf {

// Iterates over the saddle connections on a triangulation in random order.
// The distribution of the randomly selected connections should be considered
// an implementation detail but is probably nothing that can be relied on for
// statistical purposes. Roughly, we walk the triangulation randomly and return
// saddle connections as we see them. This process gives a strong preference to
// short connections (in particular initially.)
template <typename Surface>
class SaddleConnectionsSampleIterator : public boost::iterator_facade<SaddleConnectionsSampleIterator<Surface>, const SaddleConnection<Surface>, boost::forward_traversal_tag> {
  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

  using T = typename Surface::Coordinate;

  template <typename... Args>
  SaddleConnectionsSampleIterator(PrivateConstructor, Args &&... args);

 public:
  // Advance the iterator to the next saddle connection.
  void increment();
  bool equal(const SaddleConnectionsSampleIterator &other) const;
  const SaddleConnection<Surface> &dereference() const;

  template <typename S>
  friend std::ostream &operator<<(std::ostream &, const SaddleConnectionsByLengthIterator<S> &);

 private:
  Copyable<SaddleConnectionsSampleIterator> self;

  friend SaddleConnectionsSample<Surface>;
  friend ImplementationOf<SaddleConnectionsSampleIterator>;
};

}  // namespace flatsurf

#endif
