/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2022 Julian Rüth
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

#ifndef LIBFLATSURF_SEGMENT_ITERATOR_HPP
#define LIBFLATSURF_SEGMENT_ITERATOR_HPP

#include <boost/iterator/iterator_categories.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <iosfwd>

#include "copyable.hpp"

namespace flatsurf {

// Iterates over a path in a surface, i.e., over a connected sequence of segments.
template <typename Surface>
class SegmentIterator : public boost::iterator_facade<PathIterator<Surface>, const Segment<Surface>&, boost::forward_traversal_tag> {
  template <typename... Args>
  SegmentIterator(PrivateConstructor, Args&&...);

 public:
  using value_type = Segment<Surface>;

  void increment();
  const value_type& dereference() const;
  bool equal(const SegmentIterator& other) const;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const PathIterator<S>&);

 private:
  Copyable<SegmentIterator> self;

  friend Path<Surface>;
  friend ImplementationOf<SegmentIterator>;
};

}  // namespace flatsurf

#endif
