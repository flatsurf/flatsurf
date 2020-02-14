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

#include "external/spimpl/spimpl.h"

#include "forward.hpp"
#include "half_edge_set_iterator.hpp"

namespace flatsurf {

class HalfEdgeSet : boost::equality_comparable<HalfEdgeSet> {
  template <typename ...Args> HalfEdgeSet(PrivateConstructor, Args&&...);

 public:
  HalfEdgeSet(const std::vector<HalfEdge>&);

  bool contains(HalfEdge) const;
  void insert(HalfEdge);
  void erase(HalfEdge);

  HalfEdgeSetIterator begin() const;
  HalfEdgeSetIterator end() const;

  bool operator==(const HalfEdgeSet&) const;

  bool disjoint(const HalfEdgeSet&) const;

  friend std::ostream& operator<<(std::ostream&, const HalfEdgeSet&);

 private:
  using Implementation = ::flatsurf::Implementation<HalfEdgeSet>;
  spimpl::impl_ptr<Implementation> impl;
  friend Implementation;
  friend HalfEdgeSetIterator;
  friend ::flatsurf::Implementation<HalfEdgeSetIterator>;
};

HalfEdgeSetIterator begin(const HalfEdgeSet&);

HalfEdgeSetIterator end(const HalfEdgeSet&);

}

#endif
