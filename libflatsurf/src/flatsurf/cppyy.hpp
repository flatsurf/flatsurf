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

#ifndef LIBFLATSURF_CPPYY_HPP
#define LIBFLATSURF_CPPYY_HPP

#include <e-antic/renfxx_fwd.h>
#include <boost/lexical_cast.hpp>
#include <exact-real/forward.hpp>
#include <intervalxt/length.hpp>
#include <iosfwd>
#include <memory>

#include "flatsurf/flat_triangulation.hpp"
#include "flatsurf/flat_triangulation_combinatorial.hpp"
#include "flatsurf/forward.hpp"
#include "flatsurf/half_edge.hpp"
#include "flatsurf/half_edge_map.hpp"
#include "flatsurf/saddle_connection.hpp"
#include "flatsurf/saddle_connections.hpp"
#include "flatsurf/vector.hpp"
#include "flatsurf/vector_along_triangulation.hpp"

// cppyy sometimes has trouble with rvalues, let's help it to create a FlatTriangulation
template<typename T, typename V>
auto makeFlatTriangulation(const T& vertices, const std::vector<V>& vectors) {
  return std::make_shared<flatsurf::FlatTriangulation<typename V::Coordinate>>(flatsurf::FlatTriangulationCombinatorial(vertices), vectors);
}

// Work around https://bitbucket.org/wlav/cppyy/issues/96/cannot-make-wrapper-for-a-function
extern template std::ostream &flatsurf::operator<<(std::ostream &, const flatsurf::Permutation<flatsurf::HalfEdge> &);
extern template std::ostream &flatsurf::operator<<(std::ostream &, const flatsurf::HalfEdgeMap<flatsurf::Vector<long long>> &);
extern template std::ostream &flatsurf::operator<<(std::ostream &, const flatsurf::HalfEdgeMap<flatsurf::Vector<eantic::renf_elem_class>> &);
extern template std::ostream &flatsurf::operator<<(std::ostream &, const flatsurf::FlatTriangulation<long long> &);
extern template std::ostream &flatsurf::operator<<(std::ostream &, const flatsurf::FlatTriangulation<eantic::renf_elem_class> &);
extern template std::ostream &flatsurf::operator<<(std::ostream &, const flatsurf::SaddleConnection<flatsurf::FlatTriangulation<long long>> &);
extern template std::ostream &flatsurf::operator<<(std::ostream &, const flatsurf::SaddleConnection<flatsurf::FlatTriangulation<eantic::renf_elem_class>> &);
extern template std::ostream &flatsurf::operator<<(std::ostream &, const flatsurf::SaddleConnections<flatsurf::FlatTriangulation<long long>> &);
extern template std::ostream &flatsurf::operator<<(std::ostream &, const flatsurf::SaddleConnections<flatsurf::FlatTriangulation<eantic::renf_elem_class>> &);

#endif
