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

#ifndef LIBFLATSURF_CPPYY_HPP
#define LIBFLATSURF_CPPYY_HPP

#include <iosfwd>

#include "flatsurf/flat_triangulation.hpp"
#include "flatsurf/flat_triangulation_combinatorial.hpp"
#include "flatsurf/forward.hpp"
#include "flatsurf/half_edge.hpp"
#include "flatsurf/half_edge_map.hpp"
#include "flatsurf/saddle_connection.hpp"
#include "flatsurf/saddle_connections.hpp"
#include "flatsurf/vector_along_triangulation.hpp"
#include "flatsurf/vector_along_triangulation_with_approximation.hpp"
#include "flatsurf/vector_arb.hpp"
#include "flatsurf/vector_eantic.hpp"
#include "flatsurf/vector_exactreal.hpp"
#include "flatsurf/vector_longlong.hpp"

// See https://bitbucket.org/wlav/cppyy/issues/95/lookup-of-friend-operator
namespace flatsurf {
std::ostream &operator<<(std::ostream &, const HalfEdge &);
std::ostream &operator<<(std::ostream &, const VectorLongLong &);
std::ostream &operator<<(std::ostream &, const VectorArb &);
std::ostream &operator<<(std::ostream &, const VectorEAntic &);
template <typename T>
std::ostream &operator<<(std::ostream &, const Permutation<T> &);
template <typename T>
std::ostream &operator<<(std::ostream &, const HalfEdgeMap<T> &);
template <typename Vector>
std::ostream &operator<<(std::ostream &, const FlatTriangulation<Vector> &);
template <typename Vector, typename AlongTriangulation>
std::ostream &operator<<(std::ostream &, const SaddleConnection<Vector, AlongTriangulation> &);
}  // namespace flatsurf

// cppyy does not see the operators that come out of boost/operators.hpp.
// Why exactly is not clear to me at the moment. Since they are defined as
// non-template friends inside the template classes such as addable<>, we can
// not explicitly declare them like we did with the operator<< below.
template <typename S, typename T, char op>
auto boost_binary(const S &lhs, const T &rhs) {
  if constexpr (op == '+')
    return lhs + rhs;
  else if constexpr (op == '-')
    return lhs - rhs;
  else if constexpr (op == '*')
    return lhs * rhs;
  else if constexpr (op == '/')
    return lhs / rhs;
  else {
    static_assert(flatsurf::false_v<op>, "operator not implemented");
  }
}

template <typename T>
T minus(const T &lhs) {
  return -lhs;
}

// Work around https://bitbucket.org/wlav/cppyy/issues/96/cannot-make-wrapper-for-a-function
extern template std::ostream &flatsurf::operator<<(std::ostream &, const flatsurf::Permutation<flatsurf::HalfEdge> &);
extern template std::ostream &flatsurf::operator<<(std::ostream &, const flatsurf::HalfEdgeMap<flatsurf::VectorLongLong> &);
extern template std::ostream &flatsurf::operator<<(std::ostream &, const flatsurf::HalfEdgeMap<flatsurf::VectorEAntic> &);
extern template std::ostream &flatsurf::operator<<(std::ostream &, const flatsurf::FlatTriangulation<flatsurf::VectorLongLong> &);
extern template std::ostream &flatsurf::operator<<(std::ostream &, const flatsurf::FlatTriangulation<flatsurf::VectorEAntic> &);
extern template std::ostream &flatsurf::operator<<(std::ostream &, const flatsurf::SaddleConnection<flatsurf::VectorLongLong> &);
extern template std::ostream &flatsurf::operator<<(std::ostream &, const flatsurf::SaddleConnection<flatsurf::VectorEAntic> &);

#endif
