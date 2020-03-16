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

#ifndef LIBFLATSURF_EDGE_SET_HPP
#define LIBFLATSURF_EDGE_SET_HPP

#include <iosfwd>
#include <vector>

#include <boost/operators.hpp>

#include "external/spimpl/spimpl.h"

#include "forward.hpp"
#include "edge_set_iterator.hpp"

namespace flatsurf {

class EdgeSet : boost::equality_comparable<EdgeSet> {
  template <typename ...Args> EdgeSet(PrivateConstructor, Args&&...);

 public:
  EdgeSet();
  EdgeSet(const std::vector<Edge>&);

  bool contains(Edge) const;
  void insert(Edge);
  void erase(Edge);

  EdgeSetIterator begin() const;
  EdgeSetIterator end() const;

  bool empty() const;
  size_t size() const;

  bool operator==(const EdgeSet&) const;

  bool disjoint(const EdgeSet&) const;

  friend std::ostream& operator<<(std::ostream&, const EdgeSet&);

 private:
  using Implementation = ImplementationOf<EdgeSet>;
  spimpl::impl_ptr<Implementation> impl;
  friend Implementation;
  friend EdgeSetIterator;
  friend ImplementationOf<EdgeSetIterator>;
};

EdgeSetIterator begin(const EdgeSet&);

EdgeSetIterator end(const EdgeSet&);

}

#endif
