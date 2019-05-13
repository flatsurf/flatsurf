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

#include <algorithm>
#include <memory>
#include <ostream>
#include <vector>

#include "flatsurf/ccw.hpp"
#include "flatsurf/flat_triangulation.hpp"
#include "flatsurf/half_edge.hpp"
#include "flatsurf/interval_exchange_transformation.hpp"
#include "flatsurf/orientation.hpp"

using std::any_of;
using std::find_if;
using std::insert_iterator;
using std::inserter;
using std::next;
using std::ostream;
using std::shared_ptr;
using std::vector;

namespace flatsurf {
namespace {
enum class TRIANGLE {
  BACKWARD,
  FORWARD,
  LEFT_VERTICAL,
  RIGHT_VERTICAL,
};

template <typename Vector>
TRIANGLE classifyFace(HalfEdge face, const FlatTriangulation<Vector>& parent,
                      const Vector& vertical) {
  int topEdges = 0;

  for (int i = 0; i < 3; i++) {
    switch (vertical.ccw(parent.fromEdge(face))) {
      case CCW::COLLINEAR:
        switch (vertical.orientation(parent.fromEdge(face))) {
          case ORIENTATION::SAME:
            return TRIANGLE::LEFT_VERTICAL;
          case ORIENTATION::OPPOSITE:
            return TRIANGLE::RIGHT_VERTICAL;
          default:
            throw std::logic_error(
                "edge vector cannot be collinear and orthogonal at the same "
                "time");
        }
      case CCW::CLOCKWISE:
        break;
      case CCW::COUNTERCLOCKWISE:
        topEdges++;
        break;
    }

    face = parent.nextInFace(face);
  }

  if (topEdges == 1) {
    return TRIANGLE::FORWARD;
  } else {
    assert(topEdges == 2);
    return TRIANGLE::BACKWARD;
  }
}

template <typename Vector>
bool large(HalfEdge e, const FlatTriangulation<Vector>& parent,
           const Vector& vertical) {
  return classifyFace(e, parent, vertical) == TRIANGLE::FORWARD &&
         classifyFace(-e, parent, vertical) == TRIANGLE::BACKWARD;
}

template <typename Vector>
HalfEdge makeUniqueLargeEdge(FlatTriangulation<Vector>& parent,
                             const Vector& vertical) {
  auto source =
      find_if(parent.halfEdges().begin(), parent.halfEdges().end(),
              [&](const HalfEdge e) {
                return large(e, parent, vertical) &&
                       vertical.ccw(parent.fromEdge(e)) == CCW::CLOCKWISE;
              });

  if (source == parent.halfEdges().end())
    throw std::logic_error("not implemented - no large edge in triangulation");

  while (true) {
    auto largeEdge = find_if(parent.halfEdges().begin(),
                             parent.halfEdges().end(), [&](const HalfEdge e) {
                               return e != *source && e != -*source &&
                                      large(e, parent, vertical);
                             });

    if (largeEdge == parent.halfEdges().end()) break;

    parent.flip(*largeEdge);
  }

  assert(large(*source, parent, vertical) &&
         "largeness of the initial edge does not change by flipping other "
         "large edges");

  if (any_of(parent.halfEdges().begin(), parent.halfEdges().end(),
             [&](const HalfEdge e) {
               return vertical.ccw(parent.fromEdge(e)) == CCW::COLLINEAR;
             }))
    throw std::logic_error("not implemented - vertical edge in triangulation");

  return *source;
}

template <typename Vector>
void makeContour(insert_iterator<vector<HalfEdge>> target,
                 const HalfEdge source, const FlatTriangulation<Vector>& parent,
                 const Vector& vertical) {
  switch (classifyFace(source, parent, vertical)) {
    case TRIANGLE::BACKWARD:
      makeContour(target, -parent.nextInFace(parent.nextInFace(source)), parent,
                  vertical);
      makeContour(target, -parent.nextInFace(source), parent, vertical);
      break;
    case TRIANGLE::FORWARD:
      target = source;
      break;
    case TRIANGLE::LEFT_VERTICAL:
    case TRIANGLE::RIGHT_VERTICAL:
      throw std::logic_error("not implemented - vertical edge");
      break;
  }
}

}  // namespace

template <typename Vector>
struct IntervalExchangeTransformation<Vector>::Implementation {
  Implementation(const FlatTriangulation<Vector>& original,
                 const Vector& vertical) {
    auto parent = original.clone();
    HalfEdge source = makeUniqueLargeEdge(parent, vertical);
    vector<HalfEdge> top, bottom;
    makeContour(inserter(top, next(top.begin())), source, parent, vertical);
    makeContour(inserter(bottom, next(bottom.begin())), -source, parent,
                -vertical);
    reverse(bottom.begin(), bottom.end());
    transform(bottom.begin(), bottom.end(), bottom.begin(),
              [](HalfEdge e) { return -e; });
  }
};

template <typename Vector>
IntervalExchangeTransformation<Vector>::IntervalExchangeTransformation(
    FlatTriangulation<Vector>& parent, const Vector& vertical)
    : impl(spimpl::make_unique_impl<Implementation>(parent, vertical)) {}

template <typename Vector>
ostream& operator<<(ostream&, const IntervalExchangeTransformation<Vector>&) {
  throw std::logic_error(
      "not implemented IntervalExchangeTransformation::operator<<");
}
}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include <exact-real/number_field_traits.hpp>
#include "flatsurf/vector_eantic.hpp"
#include "flatsurf/vector_exactreal.hpp"
#include "flatsurf/vector_longlong.hpp"

using namespace flatsurf;

template struct flatsurf::IntervalExchangeTransformation<VectorLongLong>;
template ostream& flatsurf::operator<<(
    ostream&, const IntervalExchangeTransformation<VectorLongLong>&);
template struct flatsurf::IntervalExchangeTransformation<VectorEAntic>;
template ostream& flatsurf::operator<<(
    ostream&, const IntervalExchangeTransformation<VectorEAntic>&);
template struct flatsurf::IntervalExchangeTransformation<
    VectorExactReal<exactreal::NumberFieldTraits>>;
template ostream& flatsurf::operator<<(
    ostream&, const IntervalExchangeTransformation<
                  VectorExactReal<exactreal::NumberFieldTraits>>&);
