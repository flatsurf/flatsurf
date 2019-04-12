/**********************************************************************
 *  This file is part of Flatsurf.
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
 *  along with Flatsurf. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#include "libflatsurf/saddle_connection.hpp"

namespace flatsurf {
template <typename Vector, typename AlongTriangulation>
SaddleConnection<Vector, AlongTriangulation>::SaddleConnection(
    const FlatTriangulation<Vector>& surface, const AlongTriangulation& vector,
    const Vertex& source, const Vertex& target)
    : surface(surface), vector(vector), source(source), target(target) {}
}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include <exact-real/number_field_traits.hpp>
#include "libflatsurf/vector_eantic.hpp"
#include "libflatsurf/vector_exactreal.hpp"
#include "libflatsurf/vector_longlong.hpp"
using namespace flatsurf;

template struct flatsurf::SaddleConnection<VectorLongLong>;
template struct flatsurf::SaddleConnection<VectorEAntic>;
template struct flatsurf::SaddleConnection<
    VectorEAntic, VectorAlongTriangulation<VectorEAntic>>;
template struct flatsurf::SaddleConnection<
    VectorExactReal<exactreal::NumberFieldTraits>>;
