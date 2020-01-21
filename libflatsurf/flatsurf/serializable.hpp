/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Julian Rüth
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

#ifndef LIBFLATSURF_SERIALIZABLE_HPP
#define LIBFLATSURF_SERIALIZABLE_HPP

#include "forward.hpp"

namespace flatsurf {

// Common base class to add Cereal serialization.
// This is needed for generic classes since we cannot currently tell cereal to
// pick up free save/load functions if they are generic. (This is essentially
// the Barton-Nackman trick to go from a free function with limited
// specialization possibilities to the more powerful partial specialization
// that types provide.)
template <typename T>
class Serializable {
 public:

  template <typename Archive>
  friend void save(Archive & archive, const T& self) { Serialization<T>().save(archive, self); }
  template <typename Archive>
  friend void load(Archive & archive, T& self) { Serialization<T>().load(archive, self); }
};

}

#endif
