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

#ifndef LIBFLATSURF_LENGTH_ALONG_TRIANGULATION_HPP
#define LIBFLATSURF_LENGTH_ALONG_TRIANGULATION_HPP

#include <gmpxx.h>
#include <boost/operators.hpp>
#include "external/spimpl/spimpl.h"

#include "flatsurf/forward.hpp"

namespace flatsurf {
// Implements intervalxt::Length of half edges after projection to a
// one-dimensional subspace of ℝ².
template <typename T>
class LengthAlongTriangulation : boost::totally_ordered<LengthAlongTriangulation<T>>, boost::additive<LengthAlongTriangulation<T>>, boost::multipliable<LengthAlongTriangulation<T>, mpz_class> {
 public:
  using Quotient = mpz_class;
  using Surface = FlatTriangulation<T>;

  LengthAlongTriangulation();
  LengthAlongTriangulation(Surface const *, Vector<T> const * horizontal, const HalfEdge);

  bool operator==(const LengthAlongTriangulation&) const;
  bool operator<(const LengthAlongTriangulation&) const;
  LengthAlongTriangulation& operator+=(const LengthAlongTriangulation&);
  LengthAlongTriangulation& operator-=(const LengthAlongTriangulation&);
  LengthAlongTriangulation& operator*=(const Quotient&);

  explicit operator bool() const noexcept;

  // Return the floor of the division of the this length by the argument.
  Quotient operator/(const LengthAlongTriangulation&);

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const LengthAlongTriangulation<S>&);

 private:
  class Implementation;
  spimpl::impl_ptr<Implementation> impl;
};
}  // namespace flatsurf

#endif
