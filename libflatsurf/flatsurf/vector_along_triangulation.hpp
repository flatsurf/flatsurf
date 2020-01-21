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
#ifndef LIBFLATSURF_VECTOR_ALONG_TRIANGULATION_HPP
#define LIBFLATSURF_VECTOR_ALONG_TRIANGULATION_HPP

#include <boost/operators.hpp>
#include <vector>
#include "external/spimpl/spimpl.h"
#include "gmpxx.h"

#include "forward.hpp"

#include "detail/vector_exact.hpp"
#include "detail/vector_with_error.hpp"

namespace flatsurf {
// A generic implementation of a vector in ℝ² that is a sum of half edges of a
// flat triangulation.
template <typename T, typename Approximation, typename Surf>
class VectorAlongTriangulation : public std::conditional_t<std::is_base_of_v<detail::VectorExact<Vector<T>, T>, Vector<T>>, detail::VectorExact<VectorAlongTriangulation<T, Approximation, Surf>, T>, detail::VectorWithError<VectorAlongTriangulation<T, Approximation, Surf>>>,
                                 boost::additive<VectorAlongTriangulation<T, Approximation, Surf>, HalfEdge>,
                                 boost::additive<VectorAlongTriangulation<T, Approximation, Surf>, HalfEdgeMap<int>> {

  static_assert(std::is_same_v<Surf, std::decay_t<Surf>>, "type must not have modifiers such as const");

  using Base = std::conditional_t<std::is_base_of_v<detail::VectorExact<Vector<T>, T>, Vector<T>>, detail::VectorExact<VectorAlongTriangulation<T, Approximation, Surf>, T>, detail::VectorWithError<VectorAlongTriangulation<T, Approximation, Surf>>>;

 public:
  using Coordinate = T;
  using Surface = Surf;

  explicit VectorAlongTriangulation(const std::shared_ptr<const Surface> &);
  VectorAlongTriangulation(const std::shared_ptr<const Surface> &, const std::vector<HalfEdge> &);
  VectorAlongTriangulation(const std::shared_ptr<const Surface> &, const HalfEdgeMap<int> &coefficients);
  VectorAlongTriangulation(const std::shared_ptr<const Surface> &, const HalfEdgeMap<mpz_class> &coefficients);

  VectorAlongTriangulation &operator+=(const HalfEdge);
  VectorAlongTriangulation &operator-=(const HalfEdge);
  VectorAlongTriangulation &operator+=(const HalfEdgeMap<int> &);
  VectorAlongTriangulation &operator-=(const HalfEdgeMap<int> &);
  VectorAlongTriangulation &operator+=(const HalfEdgeMap<mpz_class> &);
  VectorAlongTriangulation &operator-=(const HalfEdgeMap<mpz_class> &);
  using Base::operator+=;
  using Base::operator-=;

  operator Vector<T>() const noexcept;

  template <typename S, typename Approx, typename Su>
  friend std::ostream &operator<<(std::ostream &, const VectorAlongTriangulation<S, Approx, Su> &);

 private:
  friend detail::VectorBase<VectorAlongTriangulation<T, Approximation>>;
  friend detail::VectorExact<VectorAlongTriangulation<T, Approximation>, T>;
  friend detail::VectorWithError<VectorAlongTriangulation<T, Approximation>>;
  class Implementation;
  spimpl::impl_ptr<Implementation> impl;
};
}  // namespace flatsurf

#endif
