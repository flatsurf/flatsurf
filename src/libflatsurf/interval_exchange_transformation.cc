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
#include <map>

#include <boost/range/adaptors.hpp>
#include <boost/lexical_cast.hpp>
#include <intervalxt/interval_exchange_transformation.hpp>
#include <intervalxt/label.hpp>

#include "flatsurf/ccw.hpp"
#include "flatsurf/flat_triangulation.hpp"
#include "flatsurf/half_edge.hpp"
#include "flatsurf/interval_exchange_transformation.hpp"
#include "flatsurf/length_along_triangulation.hpp"
#include "flatsurf/vector_along_triangulation.hpp"
#include "flatsurf/orientation.hpp"
#include "flatsurf/vector.hpp"
#include "util/as_vector.ipp"

#include "util/union_join.ipp"

using std::any_of;
using std::find_if;
using std::insert_iterator;
using std::inserter;
using std::next;
using std::ostream;
using std::shared_ptr;
using std::vector;
using std::map;

using boost::adaptors::transformed;

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
        topEdges++;
        break;
      case CCW::COUNTERCLOCKWISE:
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
  return vertical.ccw(parent.fromEdge(e)) == CCW::CLOCKWISE &&
         classifyFace(e, parent, vertical) == TRIANGLE::FORWARD &&
         classifyFace(-e, parent, vertical) == TRIANGLE::BACKWARD;
}

template <typename T>
HalfEdge makeUniqueLargeEdge(FlatTriangulation<T>& parent, const Vector<T>& vertical, const std::set<HalfEdge>& component) {
  assert(component.size());
  auto source =
      find_if(component.begin(), component.end(),
              [&](const HalfEdge e) {
                // We can start from an edge at the bottom of its face if it is:
                // * A large edge
                // * A non-vertical edge in a right vertical triangle
                return large(e, parent, vertical) ||
                  (vertical.ccw(parent.fromEdge(parent.nextInFace(e))) == CCW::COLLINEAR && vertical.ccw(parent.fromEdge(e)) == CCW::CLOCKWISE);
              });

  if (source == component.end())
    throw std::logic_error("not implemented: no large edges & no vertical edges; something is wrong.");

  // Eliminate other large edges
  while (true) {
    auto largeEdge = find_if(component.begin(), component.end(),
                             [&](const HalfEdge e) {
                               return e != *source && e != -*source &&
                                      large(e, parent, vertical);
                             });

    if (largeEdge == component.end()) break;

    parent.flip(*largeEdge);
  }

  return *source;
}

template <typename T>
void makeContour(insert_iterator<vector<HalfEdge>> target,
                 const HalfEdge source, const FlatTriangulation<T>& parent,
                 const Vector<T>& vertical, std::set<HalfEdge>& contourEdges) {
  auto addToContour = [&] () {
    target = source;
    // If we ever stumble upon this edge or its reverse, it must be part of the
    // contour.
    contourEdges.insert(source);
    contourEdges.insert(-source);
  };

  if (contourEdges.find(source) != contourEdges.end()) {
    addToContour();
    return;
  }

  std::vector<HalfEdge> recurseAcross;
  switch (classifyFace(source, parent, vertical)) {
    case TRIANGLE::BACKWARD:
      // In a backward triangle, we recurse into both edges on the top.
      recurseAcross.push_back(parent.nextInFace(parent.nextInFace(source)));
      recurseAcross.push_back(parent.nextInFace(source));
      break;
    case TRIANGLE::FORWARD:
      // In a forward triangle, we backtrack but log the edge we just crossed.
      addToContour();
      break;
    case TRIANGLE::LEFT_VERTICAL:
      // If we happen to see source again in the same direction, the surface is
      // a cylinder of vertical faces. Make sure we abort the recursion then.
      contourEdges.insert(source);
      recurseAcross.push_back(parent.nextInFace(source));
      break;
    case TRIANGLE::RIGHT_VERTICAL:
      // If we happen to see source again in the same direction, the surface is
      // a cylinder of vertical faces. Make sure we abort the recursion then.
      contourEdges.insert(source);
      recurseAcross.push_back(parent.nextInFace(parent.nextInFace(source)));
      break;
  }

  for (auto e : recurseAcross) {
    makeContour(target, -e, parent, vertical, contourEdges);
  }
}

}  // namespace

template <typename T>
class IntervalExchangeTransformation<T>::Implementation {
  using Length = LengthAlongTriangulation<T>;
  using Label = intervalxt::Label<Length>;
  using IET = intervalxt::IntervalExchangeTransformation<Length>;
  FlatTriangulation<T> parent;
  std::vector<IET> iets;

  static std::vector<IET> create(FlatTriangulation<T>& parent, const Vector<T>& vertical, const std::set<HalfEdge>& component) {
    HalfEdge source = makeUniqueLargeEdge(parent, vertical, component);

    // TODO: Move this to a separate method
    // Check whether vertical edges disconnect the surface
    map<HalfEdge, std::unique_ptr<UnionJoin<HalfEdge>>> components;
    for (auto e : component)
      components[e] = std::make_unique<UnionJoin<HalfEdge>>(e);
    for (auto e : component) {
      // Each half edge is in the same component as the other half edges in
      // its face.
      components[e]->join(*components[parent.nextInFace(e)]);
      components[e]->join(*components[parent.nextInFace(parent.nextInFace(e))]);
      // If the edge is not vertical, the two opposite half edges are in the
      // same component.
      if (parent.fromEdge(e).ccw(vertical) != CCW::COLLINEAR) {
        components[e]->join(*components[-e]);
      }
    }

    std::map<HalfEdge, std::set<HalfEdge>> newComponents;
    for (auto e : component) {
      newComponents[components[e]->representative()].insert(e);
    }

    // If there is multiple components, we call ourself for each component.
    if (newComponents.size() > 1) {
      throw std::logic_error("not implemented: recursive call in IntervalExchangeTransformation::Implementation::create()");
    }

    // We build the top contour by starting from our single wide edge and going
    // up. When we enter a backward triangle, we cross over the two top edges
    // and recurse. When we enter a forward triangle, we have found a piece of
    // the contour; the opposite search, going from the wide edge down instead
    // of up, will stop at the same edge.
    // This discussion ignores vertical edges so far which make this much more
    // complicated. In general, we treat faces with a vertical edge as backward
    // faces and recurse across the non-vertical edge. However, there are two
    // problems with this: for one, the top and the bottom contour now don't
    // contain the same edges anymore, as they find different forward faces in
    // sequences of vertical faces. And then, if all faces are vertical, the
    // recursion just won't stop. Therefore we need to identify edges of
    // vertical faces and do some extra bookkeeping to work around these two
    // issues.
    std::set<HalfEdge> verticalFaces;

    std::vector<HalfEdge> top;
    makeContour(inserter(top, top.end()), source, parent, vertical, verticalFaces);

    std::vector<HalfEdge> bottom;
    makeContour(inserter(bottom, bottom.end()), -source, parent, -vertical, verticalFaces);
    reverse(bottom.begin(), bottom.end());
    transform(bottom.begin(), bottom.end(), bottom.begin(), [](HalfEdge e) { return -e; });
    assert(std::set<HalfEdge>(bottom.begin(), bottom.end()) == std::set<HalfEdge>(top.begin(), top.end()) && "top & bottom contour must contain the same half edges");

    // For the final IntervalExchangeTransformation, the top/bottom intervals
    // are labelled by the horizontal sections of the top/bottom contour.
    auto squashed = parent.projection(vertical.perpendicular());

    map<HalfEdge, Label> labels;
    for (auto e : component)
      labels[e] = Label(Length(squashed, e));

    auto ret = std::vector<IET>();
    ret.emplace_back(IET(
        as_vector(top | transformed([&](HalfEdge e) { return labels[e]; })),
        as_vector(bottom | transformed([&](HalfEdge e) { return labels[e]; }))));
    return ret;
  }

 public:
  Implementation(const FlatTriangulation<T>& parent, const Vector<T>& vertical) : parent(std::move(parent.clone())), iets(create(this->parent, vertical, std::set<HalfEdge>(this->parent.halfEdges().begin(), this->parent.halfEdges().end()))) {}
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
