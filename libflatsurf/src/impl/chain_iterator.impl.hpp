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

#ifndef LIBFLATSURF_CHAIN_ITERATOR_IMPL_HPP
#define LIBFLATSURF_CHAIN_ITERATOR_IMPL_HPP

#include "../../flatsurf/chain_iterator.hpp"

namespace flatsurf {

template <typename Surface>
class ImplementationOf<ChainIterator<Surface>> {
 public:
  ImplementationOf(const Chain<Surface>*, int pos = 0);

  static ChainIterator<Surface> begin(const Chain<Surface>*);
  static ChainIterator<Surface> end(const Chain<Surface>*);

  const Chain<Surface>* parent;
  std::pair<Edge, const mpz_class*> current;

 private:
  // Return the index of the first non-zero entry in the chain after pos.
  // Return the number of edges if none could be found.
  static size_t findNext(const Chain<Surface>*, int pos);
  static std::pair<Edge, const mpz_class*> make(const Chain<Surface>*, size_t pos);

  friend ChainIterator<Surface>;
};

template <typename Surface>
template <typename... Args>
ChainIterator<Surface>::ChainIterator(PrivateConstructor, Args&&... args) :
  impl(spimpl::make_impl<Implementation>(std::forward<Args>(args)...)) {}

}  // namespace flatsurf

#endif
