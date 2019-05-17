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
#include "flatsurf/forward.hpp"
#include "flatsurf/half_edge.hpp"
#include "flatsurf/saddle_connections.hpp"
#include "flatsurf/vector_along_triangulation.hpp"
#include "flatsurf/vector_along_triangulation_with_approximation.hpp"
#include "flatsurf/vector_arb.hpp"
#include "flatsurf/vector_eantic.hpp"
#include "flatsurf/vector_exactreal.hpp"
#include "flatsurf/vector_longlong.hpp"

// As of early 2019, cppyy does not appear to honour
// https://en.wikipedia.org/wiki/Argument-dependent_name_lookup for operators.
// So we need to declare the operator<< in the global namespace, otherwise cppyy
// is ignoring it in str() unfortunately. At the same time, we can not only
// declare the global operator as then boost's lexical_cast does not see it,
// strangely.
std::ostream &operator<<(std::ostream &, const flatsurf::HalfEdge &);
std::ostream &operator<<(std::ostream &, const flatsurf::VectorLongLong &);
std::ostream &operator<<(std::ostream &, const flatsurf::VectorArb &);
std::ostream &operator<<(std::ostream &, const flatsurf::VectorEAntic &);
std::ostream &operator<<(std::ostream &,
                         const flatsurf::Permutation<flatsurf::HalfEdge> &);
std::ostream &
operator<<(std::ostream &,
           const flatsurf::HalfEdgeMap<flatsurf::VectorLongLong> &);
std::ostream &operator<<(std::ostream &,
                         const flatsurf::HalfEdgeMap<flatsurf::VectorEAntic> &);
std::ostream &
operator<<(std::ostream &,
           const flatsurf::FlatTriangulation<flatsurf::VectorLongLong> &);
std::ostream &
operator<<(std::ostream &,
           const flatsurf::FlatTriangulation<flatsurf::VectorEAntic> &);

#endif
