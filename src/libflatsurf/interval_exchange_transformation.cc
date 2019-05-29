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
#include "flatsurf/vector.hpp"

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

template <typename T>
TRIANGLE classifyFace(HalfEdge face, const FlatTriangulation<T>& parent,
                      const Vector<T>& vertical) {
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

template <typename T>
bool large(HalfEdge e, const FlatTriangulation<T>& parent,
           const Vector<T>& vertical) {
  return classifyFace(e, parent, vertical) == TRIANGLE::FORWARD &&
         classifyFace(-e, parent, vertical) == TRIANGLE::BACKWARD;
}

template <typename T>
HalfEdge makeUniqueLargeEdge(FlatTriangulation<T>& parent,
                             const Vector<T>& vertical) {
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

template <typename T>
void makeContour(insert_iterator<vector<HalfEdge>> target,
                 const HalfEdge source, const FlatTriangulation<T>& parent,
                 const Vector<T>& vertical) {
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

template <typename T>
class IntervalExchangeTransformation<T>::Implementation {
 public:
  Implementation(const FlatTriangulation<T>& original, const Vector<T>& vertical) {
    auto parent = original.clone();
    HalfEdge source = makeUniqueLargeEdge(parent, vertical);
    vector<HalfEdge> top, bottom;
    makeContour(inserter(top, next(top.begin())), source, parent, vertical);
    makeContour(inserter(bottom, next(bottom.begin())), -source, parent, -vertical);
    reverse(bottom.begin(), bottom.end());
    transform(bottom.begin(), bottom.end(), bottom.begin(), [](HalfEdge e) { return -e; });
  }
};

template <typename T>
IntervalExchangeTransformation<T>::IntervalExchangeTransformation(
    FlatTriangulation<T>& parent, const Vector<T>& vertical)
    : impl(spimpl::make_unique_impl<Implementation>(parent, vertical)) {}

template <typename T>
ostream& operator<<(ostream&, const IntervalExchangeTransformation<T>&) {
  throw std::logic_error("not implemented IntervalExchangeTransformation::operator<<");
}
}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include <e-antic/renfxx.h>
#include <exact-real/element.hpp>
#include <exact-real/integer_ring_traits.hpp>
#include <exact-real/number_field_traits.hpp>
#include <exact-real/rational_field_traits.hpp>

using namespace flatsurf;

template class flatsurf::IntervalExchangeTransformation<long long>;
template ostream& flatsurf::operator<<(ostream&, const IntervalExchangeTransformation<long long>&);
template class flatsurf::IntervalExchangeTransformation<eantic::renf_elem_class>;
template ostream& flatsurf::operator<<(ostream&, const IntervalExchangeTransformation<eantic::renf_elem_class>&);
template class flatsurf::IntervalExchangeTransformation<exactreal::Element<exactreal::IntegerRingTraits>>;
template ostream& flatsurf::operator<<(ostream&, const IntervalExchangeTransformation<exactreal::Element<exactreal::IntegerRingTraits>>&);
template class flatsurf::IntervalExchangeTransformation<exactreal::Element<exactreal::RationalFieldTraits>>;
template ostream& flatsurf::operator<<(ostream&, const IntervalExchangeTransformation<exactreal::Element<exactreal::RationalFieldTraits>>&);
template class flatsurf::IntervalExchangeTransformation<exactreal::Element<exactreal::NumberFieldTraits>>;
template ostream& flatsurf::operator<<(ostream&, const IntervalExchangeTransformation<exactreal::Element<exactreal::NumberFieldTraits>>&);
