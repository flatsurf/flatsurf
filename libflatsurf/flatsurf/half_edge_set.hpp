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

#ifndef LIBFLATSURF_HALF_EDGE_SET_HPP
#define LIBFLATSURF_HALF_EDGE_SET_HPP

#include <iosfwd>
#include <vector>

#include <boost/operators.hpp>

#include "copyable.hpp"

namespace flatsurf {

// A subset of the set of half edges of a triangulation.
class HalfEdgeSet : boost::equality_comparable<HalfEdgeSet> {
 public:
  HalfEdgeSet();
  HalfEdgeSet(const std::vector<HalfEdge>&);

  bool contains(HalfEdge) const;
  void insert(HalfEdge);
  void erase(HalfEdge);

  using iterator = HalfEdgeSetIterator;

  iterator begin() const;
  iterator end() const;

  bool empty() const;
  size_t size() const;

  bool operator==(const HalfEdgeSet&) const;

  bool disjoint(const HalfEdgeSet&) const;

  friend std::ostream& operator<<(std::ostream&, const HalfEdgeSet&);

 private:
  Copyable<HalfEdgeSet> self;

  friend ImplementationOf<HalfEdgeSet>;
  friend iterator;
  friend ImplementationOf<iterator>;
};

}  // namespace flatsurf

#endif
