/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2020 Julian Rüth
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

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <ostream>
#include <vector>
#include <unordered_map>

#include "../flatsurf/ccw.hpp"
#include "../flatsurf/contour_component.hpp"
#include "../flatsurf/fmt.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/orientation.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/path_iterator.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/vertex.hpp"
#include "../flatsurf/vertical.hpp"
#include "external/rx-ranges/include/rx/ranges.hpp"
#include "impl/contour_decomposition.impl.hpp"
#include "util/assert.ipp"

namespace flatsurf {

template <typename Surface>
ContourDecomposition<Surface>::ContourDecomposition(Surface surface, const Vector<T>& vertical) :
  self(spimpl::make_unique_impl<ImplementationOf<ContourDecomposition>>(std::move(surface), vertical)) {
  LIBFLATSURF_ASSERTIONS([&]() {
    ImplementationOf<ContourDecomposition>::check(components() | rx::transform([&](const auto& component) { return component.perimeter(); }) | rx::to_vector(), Vertical(self->state->surface.uncollapsed(), vertical));
  });
}

template <typename Surface>
std::vector<ContourComponent<Surface>> ContourDecomposition<Surface>::components() const {
  std::vector<ContourComponent<Surface>> components;
  for (auto& component : self->state->components)
    components.push_back(self->state->make(&component));
  return components;
}

template <typename Surface>
const FlatTriangulationCollapsed<typename Surface::Coordinate>& ContourDecomposition<Surface>::collapsed() const {
  return self->state->surface;
}

template <typename Surface>
ImplementationOf<ContourDecomposition<Surface>>::ImplementationOf(Surface surface, const Vector<T>& vertical) :
  state(new DecompositionState(std::move(surface), vertical)) {}

template <typename Surface>
void ImplementationOf<ContourDecomposition<Surface>>::check(const std::vector<Path<FlatTriangulation<T>>>& decomposition, const Vertical<FlatTriangulation<T>>& vertical) {
  const auto& surface = vertical.surface();

  // All components are closed
  {
    for (auto& component : decomposition)
      LIBFLATSURF_CHECK(component.closed(), "Components in decomposition must be closed but " << component << " is not.");
  }

  // All Connections Show Up Once With Both Signs
  {
    std::unordered_map<SaddleConnection<FlatTriangulation<T>>, int> connections;

    for (auto& component : decomposition)
      for (auto& connection : component)
        connections[connection]++;

    for (auto& connection : connections)
      LIBFLATSURF_CHECK(connections[-connection.first] == 1, "Each connection must show up exactly once with each sign in the perimiters of the components but " << connection.first << " does show up " << connections[-connection.first] + connection.second << " times.");
  }

  // The total angle at each vertex has not changed
  {
    // Total angle at each vertex in multiples of π/2
    std::unordered_map<Vertex, int> totalAngle;

    enum CLASSIFICATION { TOP,
      BOTTOM,
      LEFT,
      RIGHT };

    const auto classify = [&](const auto& connection) {
      switch (vertical.ccw(connection)) {
        case CCW::COLLINEAR:
          switch (vertical.orientation(connection)) {
            case ORIENTATION::SAME:
              return RIGHT;
            case ORIENTATION::OPPOSITE:
              return LEFT;
            default:
              LIBFLATSURF_UNREACHABLE("connection cannot be collinear and orthogonal at the same time");
          }
        case CCW::CLOCKWISE:
          return BOTTOM;
        default:
          return TOP;
      }
    };

    const auto angle = [&](const auto& self, const auto& next) {
      switch (classify(self)) {
        case TOP:
          switch (classify(next)) {
            case TOP:
              return 2;
            case BOTTOM:
              return 0;
            case LEFT:
              return 1;
            case RIGHT:
              return 3;
          }
          break;
        case BOTTOM:
          switch (classify(next)) {
            case TOP:
              return 0;
            case BOTTOM:
              return 2;
            case LEFT:
              return 3;
            case RIGHT:
              return 1;
          }
          break;
        case LEFT:
          switch (classify(next)) {
            case TOP:
              return 3;
            case BOTTOM:
              return 1;
            case LEFT:
              return 2;
            case RIGHT:
              return 4;
          }
          break;
        case RIGHT:
          switch (classify(next)) {
            case TOP:
              return 1;
            case BOTTOM:
              return 3;
            case LEFT:
              return 4;
            case RIGHT:
              return 2;
          }
          break;
      }
      throw std::logic_error("impossible classification");
    };

    for (auto& component : decomposition) {
      auto connection = component.begin();
      auto nextConnection = component.begin();
      ++nextConnection;

      do {
        totalAngle[Vertex::source(nextConnection->source(), surface)] += angle(*connection, *nextConnection);

        ++connection;
        ++nextConnection;
      } while (connection != component.begin());
    }

    for (const auto vertex : surface.vertices()) {
      LIBFLATSURF_CHECK(totalAngle[vertex] != 0, "All marked vertices must still be present in decomposition but " << vertex << " was not found anywhere on the perimeters " << fmt::format("[{}]", fmt::join(decomposition, ", ")));
      const auto formatAngle = [](const int angle) {
        if (angle % 2)
          return fmt::format("{}·π/2", angle);
        else
          return fmt::format("{}π", angle / 2);
      };
      LIBFLATSURF_CHECK(totalAngle[vertex] == 4 * surface.angle(vertex), "Total angle at each vertex must not change in decomposition but " << vertex << " has angle " << formatAngle(totalAngle[vertex]) << " in decomposition [" << fmt::format("[{}]", fmt::join(decomposition, ", ")) << "] with vertical " << vertical << " but had angle " << formatAngle(surface.angle(vertex) * 4) << " in surface originally.");
    }
  }

  // Total Area has not Changed
  {
    const auto area = decomposition | rx::transform([](const auto& component) { return component.area(); }) | rx::sum();
    LIBFLATSURF_CHECK(area == surface.area(), "Total area of components does not match area of original surface, " << area << " != " << surface.area());
  }

  // Components do not overlap
  {
    // https://github.com/flatsurf/flatsurf/issues/150
  }
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const ContourDecomposition<Surface>& self) {
  return os << fmt::format("[{}]", fmt::join(self.components(), ", "));
}
}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), ContourDecomposition, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
