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

#include <stack>
#include <memory>
#include <unordered_set>

// TODO: Deleteme
#include <boost/lexical_cast.hpp>

#include "../flatsurf/ccw.hpp"
#include "../flatsurf/vertical.hpp"
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
HalfEdge FlowTriangulation<Surface>::halfEdge(const FlowConnection<Surface>& connection) const {
  return impl->perimeter.at(connection);
}

template <typename Surface>
ImplementationOf<FlowTriangulation<Surface>>::ImplementationOf(const FlowComponent<Surface>& component) {
  std::unordered_map<HalfEdge, Vector<T>> vectors;
  std::vector<std::tuple<HalfEdge, HalfEdge, HalfEdge>> faces;

  const auto face = [&](HalfEdge f, HalfEdge g) {
    HalfEdge e(vectors.size() / 2 + 1);

    ASSERT([&]() {
      std::unordered_set<Edge> edges;
      edges.insert(e);
      edges.insert(f);
      edges.insert(g);
      return edges.size() == 3;
    }(), "(" << e << " " << f << " " << g << ") can not form a face");

    ASSERT(vectors.find(f) != vectors.end() && vectors.find(g) != vectors.end(), "at least two of (" << e << " " << f << " " << g << ") must be known when inserting a new face");

    ASSERT(vectors[f].ccw(vectors[g]) == CCW::COUNTERCLOCKWISE, "half edges " << f << " and " << g << " do not form a convex corner");

    std::cout << "(" << e << " " << f << " " << g << ")" << std::endl;

    faces.emplace_back(e, f, g);
    vectors[-e] = vectors[f] + vectors[g];
    vectors[e] = -vectors[-e];

    return e;
  };

  // We map the contour to half edges 1, 2, …
  for (const auto& connection : component.perimeter()) {
    ASSERT(perimeter.find(connection) == perimeter.end(), "connection cannot show up more than once in a perimeter");
    const HalfEdge halfEdge = perimeter.find(-connection) == perimeter.end() ? HalfEdge(vectors.size() / 2 + 1) : -perimeter[-connection];
    perimeter[connection] = halfEdge;
    ASSERT(vectors.find(halfEdge) == vectors.end() || vectors[halfEdge] == connection.saddleConnection(), "top & bottom contour vectors are inconsistent");
    vectors[halfEdge] = connection.saddleConnection();
    vectors[-halfEdge] = -vectors[halfEdge];
  }

  {
    std::vector<std::tuple<std::string, Vector<T>, Vector<T>>> segments;

    bool bottom = true;
    int id = 0;
    int sid = 0;

    Vector<T> a;
    for (const auto& connection : component.perimeter()) {
      Vector<T> b = a + connection.saddleConnection();

      if (connection.top()) {
        bottom = false;
        id--;
        sid = 0;
      }

      std::string name = (bottom ? "b" : "t") + boost::lexical_cast<std::string>(id);
      if (connection.parallel()) {
        name += "up";
        name += boost::lexical_cast<std::string>(sid);
        sid++;
      } else if (connection.antiparallel()) {
        name += "down";
        name += boost::lexical_cast<std::string>(sid);
        sid++;
      } else if (connection.bottom()) {
        id++;
        sid = 0;
      }
      std::cout << name << "=" << perimeter[connection] << " ";
      segments.emplace_back(name, a, b);

      a = b;
    }
    std::cout << std::endl;

    ASSERT(!a, "contour is not closed");

    for (const auto& v : segments) {
      for (const auto& w : segments) {
        if (v == w) continue;

        const auto ne = [](const auto& x, const auto& y, const auto& xx, const auto& yy) {
          if (x != y && xx != yy) {
            if ((x == CCW::COLLINEAR || y == CCW::COLLINEAR) && (xx == CCW::COLLINEAR || yy == CCW::COLLINEAR))
              return false;
            return true;
          }
          return false;
        };

        const auto vavb = std::get<2>(v) - std::get<1>(v);
        const auto vawa = std::get<1>(w) - std::get<1>(v);
        const auto vawb = std::get<2>(w) - std::get<1>(v);
        const auto wawb = std::get<2>(w) - std::get<1>(w);
        const auto wava = std::get<1>(v) - std::get<1>(w);
        const auto wavb = std::get<2>(v) - std::get<1>(w);
        if (ne(vavb.ccw(vawa), vavb.ccw(vawb),wawb.ccw(wava), wawb.ccw(wavb))) {
          std::cout << std::get<0>(v) << " intersects " << std::get<0>(w) << std::endl;
        }
      }
    }
  }

  // We build the triangulation using a standard algorithm for triangulating
  // simple monotone polygons. Note that our polygon might be self-intersecting
  // and therefore not simple. However, it is monotone: at most two edges
  // intersect any vertical. For the algorithm see e.g. Mark de Berg,
  // Computational Geometry, Springer 1997 p.57. The statement there about
  // rotating the plane is a bit confusing. Simply rotating the plane could
  // make the polygon non-monotone. Instead, we are a bit more careful how we
  // order vertices that appear on a vertical to make our ordering actually
  // correspond to a strictly-monotone polygon.
  
  struct Vertex {
    Vertex(const T& x, bool bottom, HalfEdge halfEdge) : x(x), bottom(bottom), halfEdge(halfEdge) {
      CHECK_ARGUMENT(x >= 0, "all vertices must be to the right of the bottom-left vertex");
    }

    // A measure for the horizontal position of this vertex; the sort order in the monotone chain.
    T x;

    // Whether this vertex is considered to be on the monotone bottom chain.
    bool bottom;

    // The half edge leading to this vertex (if bottom) or leaving from this
    // vertex (if top).
    HalfEdge halfEdge;

    bool operator<(const Vertex& rhs) const { return x < rhs.x; }
  };
  
  // We collect the vertices on the contour accompanied by a value that
  // corresponds to their horizontal coordinate.
  std::vector<Vertex> chain;

  {
    chain.emplace_back(T(), true, HalfEdge(404));
    
    T x = T();

    for (const auto& connection : component.perimeter()) {
      const HalfEdge halfEdge = perimeter[connection];
      if (connection.top()) break;
      x += component.vertical().perpendicular(connection.saddleConnection());
      chain.emplace_back(x, true, halfEdge);
    }

    const HalfEdge bottomChainEnd = rbegin(chain)->halfEdge;
    x = T();

    for (const auto& connection : component.perimeter() | rx::reverse() | rx::to_vector()) {
      const HalfEdge halfEdge = perimeter[connection];
      if (halfEdge == bottomChainEnd) break;
      x -= component.vertical().perpendicular(connection.saddleConnection());
      chain.emplace_back(x, false, halfEdge);
    }

    std::stable_sort(begin(chain), end(chain));
  }


  {
    std::deque<Vertex> S;

    auto u = begin(chain);

    S.push_back(*u++);
    S.push_back(*u++);

    for(; u != end(chain) - 1; u++) {
      ASSERT(S.size() >= 2, "The pending triangulation funnel cannot consists of a single vertex.");
      if (S.back().bottom != u->bottom) {
        std::cout << "cross" << std::endl;
        while (S.size() > 1) {
          S.pop_front();
          Vertex v = S.front();

          if (u == end(chain) - 2 && S.size() == 1) {
            // The final face in the triangulation: All the half edges are
            // already there, we just need to add the combinatorial data for
            // this face.
            ASSERT(u->bottom && !rbegin(chain)->bottom, "Since we consider the rightmost verticals to be part of the bottom contour, the second to last edge must be a bottom edge and the last edge a top edge.");
            faces.emplace_back(v.halfEdge, u->halfEdge, rbegin(chain)->halfEdge);
            continue;
          }

          // Create a face in the triangulation by adding a cross edge.
          u->halfEdge = - (u->bottom ? face(v.halfEdge, u->halfEdge) : face(u->halfEdge, v.halfEdge));
        }
        S.front().halfEdge = HalfEdge(404);
        S.push_back(*u);
      } else {
        std::cout << "same" << std::endl;
        do {
          Vertex v = S.back();
          ASSERT(u->bottom == v.bottom, "All vertices on the stack must be on the same chain.");

          if (u == end(chain) - 2 && S.size() <= 2) {
            // The final face in the triangulation: All the half edges are
            // already there, we just need to add the combinatorial data for
            // this face.
            ASSERT(u->bottom && !rbegin(chain)->bottom, "Since we consider the rightmost verticals to be part of the bottom contour, the second to last edge must be a bottom edge and the last edge a top edge.");
            S.pop_back();
            faces.emplace_back(v.halfEdge, u->halfEdge, rbegin(chain)->halfEdge);
            break;
          }

          if (vectors[v.halfEdge].ccw(vectors[u->halfEdge]) != (u->bottom ? CCW::COUNTERCLOCKWISE : CCW::CLOCKWISE)) {
            ASSERT(u != end(chain) - 2, "no concave angles can remain from the final vertex");
            break;
          }

          // Create a face clipping this convex ear of the contour.
          S.pop_back();
          u->halfEdge = -(u->bottom ? face(v.halfEdge, u->halfEdge) : face(u->halfEdge, v.halfEdge));
        } while(S.size() > 1);

        S.push_back(*u);
      }
    }
  }

  triangulation = std::make_shared<FlatTriangulation<T>>(FlatTriangulationCombinatorial(faces), [&](const HalfEdge he) {
    return vectors[he];
  });

  ASSERT([&]() {
      std::unordered_set<HalfEdge> boundary;

      for (auto halfEdge : triangulation->halfEdges())
        if (triangulation->boundary(halfEdge))
          boundary.insert(halfEdge);

      return boundary;
    }() == [&]() {
      std::unordered_set<HalfEdge> boundary;

      for (auto connection : component.perimeter())
        if (connection.component() != (-connection).component())
          boundary.insert(-perimeter[connection]);

      return boundary;
    }(), "boundaries of flow component " << component << " and its triangulation " << *triangulation << " are inconsistent.");
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
