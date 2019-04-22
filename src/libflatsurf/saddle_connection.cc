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

#include <ostream>

#include "flatsurf/saddle_connection.hpp"

using std::ostream;

namespace flatsurf {
template <typename Vector, typename AlongTriangulation>
SaddleConnection<Vector, AlongTriangulation>::SaddleConnection(
    const FlatTriangulation<Vector> &surface, const AlongTriangulation &vector,
    const HalfEdge &source, const HalfEdge &target)
    : surface(surface), vector(vector), source(source), target(target) {}

template <typename Vector, typename AlongTriangulation>
ostream &operator<<(ostream &os,
                    const SaddleConnection<Vector, AlongTriangulation> &self) {
  return os << "SaddleConnection(" << self.source << "→" << self.target << ": "
            << self.vector << ")";
}
}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include <exact-real/number_field_traits.hpp>
#include "flatsurf/forward.hpp"
#include "flatsurf/vector_eantic.hpp"
#include "flatsurf/vector_exactreal.hpp"
#include "flatsurf/vector_longlong.hpp"
using namespace flatsurf;

template struct flatsurf::SaddleConnection<VectorLongLong>;
template ostream &flatsurf::operator<<(
    ostream &, const SaddleConnection<VectorLongLong> &);
template struct flatsurf::SaddleConnection<VectorEAntic>;
template ostream &flatsurf::operator<<(ostream &,
                                       const SaddleConnection<VectorEAntic> &);
template struct flatsurf::SaddleConnection<
    VectorEAntic, VectorAlongTriangulation<VectorEAntic>>;
template ostream &flatsurf::operator<<(
    ostream &,
    const SaddleConnection<VectorEAntic, VectorAlongTriangulation<VectorEAntic>>
        &);
template struct flatsurf::SaddleConnection<
    VectorExactReal<exactreal::NumberFieldTraits>>;
template ostream &flatsurf::operator<<(
    ostream &,
    const SaddleConnection<VectorExactReal<exactreal::NumberFieldTraits>> &);
