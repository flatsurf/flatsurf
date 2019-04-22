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
#include <vector>

#include "flatsurf/assert.hpp"
#include "flatsurf/flat_triangulation.hpp"
#include "flatsurf/half_edge.hpp"
#include "flatsurf/half_edge_map.hpp"

using std::ostream;
using std::vector;

namespace flatsurf {
template <typename Vector>
struct FlatTriangulation<Vector>::Implementation {
  Implementation(const HalfEdgeMap<Vector> &vectors) : vectors(vectors) {}

  const HalfEdgeMap<Vector> vectors;
};

template <typename Vector>
const Vector &FlatTriangulation<Vector>::fromEdge(const HalfEdge e) const {
  return impl->vectors.get(e);
}

template <typename Vector>
FlatTriangulation<Vector>::FlatTriangulation(
    FlatTriangulationCombinatorial &&combinatorial,
    const vector<Vector> &vectors)
    : FlatTriangulation(std::move(combinatorial),
                        HalfEdgeMap<Vector>(vectors)) {}

template <typename Vector>
FlatTriangulation<Vector>::FlatTriangulation(
    FlatTriangulationCombinatorial &&combinatorial,
    const HalfEdgeMap<Vector> &vectors)
    : FlatTriangulationCombinatorial(std::move(combinatorial)),
      impl(spimpl::make_unique_impl<Implementation>(vectors)) {
  // check that faces are closed
  for (size_t e = 0; e < nedges; e++) {
    HalfEdge edge(static_cast<int>(e + 1));
    auto zero = fromEdge(edge);
    edge = nextInFace(edge);
    zero += fromEdge(edge);
    edge = nextInFace(edge);
    zero += fromEdge(edge);
    CHECK_ARGUMENT(!zero, "some face is not closed");
  }
}

template <typename Vector>
ostream &operator<<(ostream &os, const FlatTriangulation<Vector> &self) {
  return os << static_cast<const FlatTriangulationCombinatorial &>(self)
            << " with vectors " << self.impl->vectors;
}
}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include <exact-real/number_field_traits.hpp>
#include "flatsurf/vector_eantic.hpp"
#include "flatsurf/vector_exactreal.hpp"
#include "flatsurf/vector_longlong.hpp"

using namespace flatsurf;

template struct flatsurf::FlatTriangulation<VectorLongLong>;
template ostream &flatsurf::operator<<(
    ostream &, const FlatTriangulation<VectorLongLong> &);
template struct flatsurf::FlatTriangulation<VectorEAntic>;
template ostream &flatsurf::operator<<(ostream &,
                                       const FlatTriangulation<VectorEAntic> &);
template struct flatsurf::FlatTriangulation<
    VectorExactReal<exactreal::NumberFieldTraits>>;
template ostream &flatsurf::operator<<(
    ostream &,
    const FlatTriangulation<VectorExactReal<exactreal::NumberFieldTraits>> &);
