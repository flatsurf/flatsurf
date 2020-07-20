/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020 Julian Rüth
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

#include <memory>
#include <unordered_set>

#include "../flatsurf/edge.hpp"
#include "../flatsurf/flat_triangulation_combinatorial.hpp"
#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/flow_component.hpp"
#include "../flatsurf/flow_connection.hpp"
#include "../flatsurf/flow_triangulation.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/saddle_connection.hpp"

#include "impl/flow_triangulation.impl.hpp"

#include "external/rx-ranges/include/rx/ranges.hpp"

#include "util/assert.ipp"

namespace flatsurf {

template <typename Surface>
template <typename... Args>
FlowTriangulation<Surface>::FlowTriangulation(PrivateConstructor, Args&&... args) :
  impl(spimpl::make_impl<Implementation>(std::forward<Args>(args)...)) {}

template <typename Surface>
std::shared_ptr<const FlatTriangulation<typename Surface::Coordinate>> FlowTriangulation<Surface>::triangulation() const {
  return impl->triangulation;
}

template <typename Surface>
ImplementationOf<FlowTriangulation<Surface>>::ImplementationOf(const FlowComponent<Surface>& component) {
  std::unordered_map<HalfEdge, Vector<T>> vectors;
  std::vector<std::tuple<HalfEdge, HalfEdge, HalfEdge>> faces;

  for (const auto& connection : component.perimeter()) {
    ASSERT(perimeter.find(connection) == perimeter.end(), "connection cannot show up more than once in a perimeter");
    HalfEdge he = perimeter.find(-connection) == perimeter.end() ? HalfEdge(vectors.size() / 2 + 1) : -perimeter[-connection];
    perimeter[connection] = he;
    vectors[he] = connection.saddleConnection();
    vectors[-he] = -vectors[he];
  }

  // We build the triangulation quite naively by walking the horizontal bits of
  // the top & bottom contour simultaneously from left to right and
  // triangulating everything that is between these two edges.
  // We know that this does not always work. In particular it usually does not
  // work when the contours are intersecting.
  const auto horizontal = component.perimeter() | rx::filter([](const auto& connection) { return !connection.vertical(); }) | rx::to_vector();
  ASSERT(horizontal.size() % 2 == 0, "horizontal connections must come in pairs since they come from intervals of an IET");

  std::vector<FlowConnection<Surface>> bottoms(begin(horizontal), begin(horizontal) + horizontal.size() / 2);
  std::vector<FlowConnection<Surface>> tops(end(horizontal) - horizontal.size() / 2, end(horizontal));
  std::reverse(begin(tops), end(tops));

  ASSERT(std::unordered_set<FlowConnection<Surface>>(begin(bottoms), end(bottoms)) == [&](){ const auto _ = (tops | rx::transform([](const auto& connection) { return -connection; }) | rx::to_vector()); return std::unordered_set<FlowConnection<Surface>>(begin(_), end(_)); }(), "top & bottom contour must contain the same connections with reversed directions");      

  const auto insert = [&](HalfEdge f, HalfEdge g) {
    HalfEdge e(vectors.size() / 2 + 1);

    ASSERT([&]() {
      std::unordered_set<Edge> edges;
      edges.insert(e);
      edges.insert(f);
      edges.insert(g);
      return edges.size() == 3;
    }(), "(" << e << ", " << f << ", " << g << ") can not form a face");

    ASSERT(vectors.find(f) != vectors.end() && vectors.find(g) != vectors.end(), "at least two of (" << e << ", " << f << ", " << g << ") must be known when inserting a new face");

    faces.emplace_back(e, f, g);
    vectors[-e] = vectors[f] + vectors[g];
    vectors[e] = -vectors[-e];

    return e;
  };

  // As we swipe over the triangulation, we track the edge that we introduced
  // to connect from the top to the bottom contour here.
  HalfEdge gap;

  for (auto bottom = begin(bottoms); bottom != end(bottoms); bottom++) {
    const bool initial = bottom == begin(bottoms);
    const bool final = bottom + 1 == end(bottoms);

    auto top = begin(tops) + (bottom - begin(bottoms));

    // We triangulate everything between the vertical edges attached to left
    // and right of bottom.
    HalfEdge bottomLeftToRight = perimeter[*bottom];
    {
      // First by connecting the bottom left to vertices on the right…
      auto bottomRight = bottom->nextInPerimeter();
      while (bottomRight.parallel()) {
        bottomLeftToRight = -insert(bottomLeftToRight, perimeter[bottomRight]);
        bottomRight = bottomRight.nextInPerimeter();
      }
      ASSERT((bottomRight == *top) == final, "only the last pair of intervals has a right that is connected");
    }
    {
      // …then by connecting the vertices on the left to the topmost vertex on the right.
      auto bottomLeft = bottom->previousInPerimeter();
      while (bottomLeft.antiparallel()) {
        bottomLeftToRight = -insert(perimeter[bottomLeft], bottomLeftToRight);
        bottomLeft = bottomLeft.previousInPerimeter();
      }
      ASSERT((bottomLeft == *top) == initial, "only the first pair of intervals has a left that is connected");
    }

    // We now do the same for the top, i.e., we triangulate everything between
    // the vertical edges attached to top.
    HalfEdge topRightToLeft = perimeter[*top];
    if (!initial) {
      // First by connecting the top right to the verticas on the left…
      auto topLeft = top->nextInPerimeter();
      while (topLeft.antiparallel()) {
        topRightToLeft = -insert(topRightToLeft, perimeter[topLeft]);
        topLeft = topLeft.nextInPerimeter();
      }
    }
    if (!final) {
      // …then by connecting the vertices on the right to the bottommost vertex on the left.
      auto topRight = top->previousInPerimeter();
      while (topRight.parallel()) {
        topRightToLeft = -insert(perimeter[topRight], topRightToLeft);
        topRight = topRight.previousInPerimeter();
      }
    }

    // Finally, triangulate the remaining quadrilateral.
    if (!initial && !final) {
      topRightToLeft = -insert(topRightToLeft, gap);
    }
    if (!final) {
      gap = -insert(topRightToLeft, bottomLeftToRight);
    }
  }

  triangulation = std::make_shared<FlatTriangulation<T>>(FlatTriangulationCombinatorial(faces), [&](const HalfEdge he) {
    return vectors[he];
  });
}

template <typename Surface>
FlowTriangulation<Surface> ImplementationOf<FlowTriangulation<Surface>>::make(const FlowComponent<Surface>& component) {
  return FlowTriangulation<Surface>(PrivateConstructor{}, component);
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const FlowTriangulation<Surface>& self) {
  return os << *self.triangulation();
}
}

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), FlowTriangulation, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
