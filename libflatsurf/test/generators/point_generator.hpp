/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2022 Julian Rüth
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

#ifndef LIBFLATSURF_TEST_GENERATORS_POINT_GENERATOR_HPP
#define LIBFLATSURF_TEST_GENERATORS_POINT_GENERATOR_HPP

#include <memory>
#include <vector>

#include "../../flatsurf/point.hpp"
#include "../../flatsurf/half_edge.hpp"
#include "../../flatsurf/vertex.hpp"
#include "../external/catch2/single_include/catch2/catch.hpp"

namespace flatsurf::test {

template <typename T>
class PointGenerator : public Catch::Generators::IGenerator<Point<FlatTriangulation<T>>> {
  std::shared_ptr<FlatTriangulation<T>> surface;
  HalfEdge face;

  enum class POINT {
    SOURCE = 0,
    MIDPOINT_EDGE = 1,
    CENTER_FACE = 2,
    OUTSIDE = 3,
    LAST = 3,
  } state;
  
  Point<FlatTriangulation<T>> current;

  Point<FlatTriangulation<T>> make(POINT kind) {
    switch (kind) {
      case POINT::SOURCE:
        return Point{*surface, Vertex::source(face, *surface)};
      case POINT::MIDPOINT_EDGE:
        return Point{*surface, face, T(1), T(1), T(0)};
      case POINT::CENTER_FACE:
        return Point{*surface, face, T(1), T(1), T(1)};
      case POINT::OUTSIDE:
        // Note that this point might not exist.
        return Point{*surface, face, T(1024), T(1024), T(-1)};
      default:
        throw std::logic_error("not implemented: PointGenerator::make()");
    }
  }

 public:
  PointGenerator(std::shared_ptr<FlatTriangulation<T>> surface, HalfEdge face) : surface(surface), face(face), state(POINT::SOURCE), current(make(state)) {
  }

  const Point<FlatTriangulation<T>>& get() const override {
    return current;
  }

  bool next() override {
    if (state == POINT::LAST)
      return false;

    state = static_cast<POINT>(static_cast<int>(state) + 1);

    current = make(state);

    return true;
  }
};

template <typename T>
Catch::Generators::GeneratorWrapper<Point<FlatTriangulation<T>>> points(std::shared_ptr<FlatTriangulation<T>> surface, const HalfEdge face) {
  return Catch::Generators::GeneratorWrapper<Point<FlatTriangulation<T>>>(std::unique_ptr<Catch::Generators::IGenerator<Point<FlatTriangulation<T>>>>(new PointGenerator<T>(surface, face)));
}

}

#endif
