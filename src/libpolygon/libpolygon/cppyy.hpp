/**********************************************************************
 *  This file is part of Polygon.
 *
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Polygon is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Polygon is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Polygon. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#ifndef LIBPOLYGON_CPPYY_HPP
#define LIBPOLYGON_CPPYY_HPP

#include <iosfwd>

#include "libpolygon/forward.hpp"
#include "libpolygon/flat_triangulation.hpp"
#include "libpolygon/saddle_connections.hpp"
#include "libpolygon/vector_longlong.hpp"
#include "libpolygon/vector_eantic.hpp"
#include "libpolygon/vector_arb.hpp"
#include "libpolygon/half_edge.hpp"
#include "libpolygon/vector_along_triangulation.hpp"
#include "libpolygon/vector_along_triangulation_with_approximation.hpp"

// As of early 2019, cppyy does not appear to honour https://en.wikipedia.org/wiki/Argument-dependent_name_lookup for operators.
// So we need to declare the operator<< in the global namespace, otherwise cppyy is ignoring it in str() unfortunately.
// At the same time, we can not only declare the global operator as then boost's lexical_cast does not see it, strangely.
std::ostream& operator<<(std::ostream&, const flatsurf::HalfEdge&);
std::ostream& operator<<(std::ostream&, const flatsurf::VectorLongLong&);
std::ostream& operator<<(std::ostream&, const flatsurf::VectorArb&);
std::ostream& operator<<(std::ostream&, const flatsurf::VectorEAntic&);
std::ostream& operator<<(std::ostream&, const flatsurf::Permutation<flatsurf::HalfEdge>&);
std::ostream& operator<<(std::ostream&, const flatsurf::HalfEdgeMap<flatsurf::VectorLongLong>&);
std::ostream& operator<<(std::ostream&, const flatsurf::HalfEdgeMap<flatsurf::VectorEAntic>&);
std::ostream& operator<<(std::ostream&, const flatsurf::FlatTriangulation<flatsurf::VectorLongLong>&);
std::ostream& operator<<(std::ostream&, const flatsurf::FlatTriangulation<flatsurf::VectorEAntic>&);

#endif
