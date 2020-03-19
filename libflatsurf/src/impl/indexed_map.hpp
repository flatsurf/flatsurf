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

#ifndef LIBFLATSURF_IMPL_INDEXED_SET_HPP
#define LIBFLATSURF_IMPL_INDEXED_SET_HPP

#include <iosfwd>
#include <vector>

#include "forward.hpp"

namespace flatsurf {

template <typename K, typename V>
class IndexedMap {
 public:
  IndexedMap();

  template <typename KK, typename VV>
  friend std::ostream& operator<<(std::ostream&, const IndexedMap<KK, VV>&);

 private:
  std::vector<V> values {};
};

}

#endif
