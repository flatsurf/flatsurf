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

#ifndef LIBFLATSURF_TEST_GENERATORS_RAY_GENERATOR_HPP
#define LIBFLATSURF_TEST_GENERATORS_RAY_GENERATOR_HPP

#include <memory>
#include <vector>

#include "../../flatsurf/ray.hpp"
#include "../../flatsurf/point.hpp"
#include "../../flatsurf/half_edge.hpp"
#include "surface_generator.hpp"
#include "../external/catch2/single_include/catch2/catch.hpp"

namespace flatsurf::test {

template <typename T>
class RayGenerator : public Catch::Generators::IGenerator<Ray<FlatTriangulation<T>>> {
  std::shared_ptr<const FlatTriangulation<T>> surface;
  HalfEdge face;

  enum class RAY {
    VERTEX_ALONG_EDGE = 0,
    EDGE_ALONG_EDGE = 1,
    EDGE_TO_FACE = 2,
    FACE_TO_FACE = 3,
    LAST = 3,
  } state;
  
  Ray<FlatTriangulation<T>> current;

  Ray<FlatTriangulation<T>> make(RAY kind) {
    switch (kind) {
      case RAY::VERTEX_ALONG_EDGE:
      case RAY::EDGE_ALONG_EDGE:
      case RAY::EDGE_TO_FACE:
      case RAY::FACE_TO_FACE:
      default:
        throw std::logic_error("not implemented: RayGenerator::make()");
    }
  }

 public:
  RayGenerator(std::shared_ptr<const FlatTriangulation<T>> surface, HalfEdge face) : surface(surface), face(face), state(RAY::VERTEX_ALONG_EDGE), current(make(state)) {
  }

  const Ray<FlatTriangulation<T>>& get() const override {
    return current;
  }

  bool next() override {
    if (state == RAY::LAST)
      return false;

    state = static_cast<RAY>(static_cast<int>(state) + 1);

    current = make(state);

    return true;
  }
};

template <typename T>
Catch::Generators::GeneratorWrapper<Ray<FlatTriangulation<T>>> rays(NamedSurface<T> surface, const HalfEdge face) {
  return Catch::Generators::GeneratorWrapper<Ray<FlatTriangulation<T>>>(std::unique_ptr<Catch::Generators::IGenerator<Ray<FlatTriangulation<T>>>>(new RayGenerator<T>(surface, face)));
}

}
#endif
