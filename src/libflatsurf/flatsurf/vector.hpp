/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
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
#ifndef LIBFLATSURF_VECTOR_HPP
#define LIBFLATSURF_VECTOR_HPP

#include <exact-real/forward.hpp>
#include "external/spimpl/spimpl.h"

#include "flatsurf/detail/vector_exact.hpp"
#include "flatsurf/detail/vector_with_error.hpp"
#include "flatsurf/forward.hpp"

namespace flatsurf {
// A vector in ℝ² whose coordinates are of type T.
template <typename T>
class Vector : public std::conditional_t<std::is_same_v<T, exactreal::Arb>, detail::VectorWithError<Vector<T>>, detail::VectorExact<Vector<T>, T>> {
 public:
  using Coordinate = T;

  Vector();
  Vector(const Coordinate& x, const Coordinate& y);

 private:
  friend detail::VectorExact<Vector<T>, T>;
  friend detail::VectorWithError<Vector<T>>;
  friend detail::VectorBase<Vector<T>>;
  template <typename V>
  friend std::ostream& detail::operator<<(std::ostream&, const detail::VectorBase<V>&);

  class Implementation;
  spimpl::impl_ptr<Implementation> impl;
};
}  // namespace flatsurf

#endif
