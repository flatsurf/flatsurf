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

#ifndef LIBFLATSURF_CHAIN_ITERATOR_HPP
#define LIBFLATSURF_CHAIN_ITERATOR_HPP

#include <gmpxx.h>

#include <boost/iterator/iterator_categories.hpp>
#include <boost/iterator/iterator_facade.hpp>

#include "copyable.hpp"

namespace flatsurf {

// An iterator over a Chain on a surface, i.e., an iterator over the
// coefficient, edge pairs of the formal sum representing a chain.
template <typename Surface>
class ChainIterator : public boost::iterator_facade<ChainIterator<Surface>, const std::pair<Edge, const mpz_class*>&, boost::forward_traversal_tag> {
  template <typename... Args>
  ChainIterator(PrivateConstructor, Args&&...);

 public:
  using value_type = std::pair<Edge, const mpz_class*>;

  void increment();
  const value_type& dereference() const;
  bool equal(const ChainIterator& other) const;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const ChainIterator<S>&);

 private:
  Copyable<ChainIterator> self;

  friend ImplementationOf<ChainIterator>;
};

}  // namespace flatsurf

#endif
