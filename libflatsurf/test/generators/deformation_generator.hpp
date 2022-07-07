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

#ifndef LIBFLATSURF_TEST_GENERATORS_DEFORMATION_GENERATORS_HPP
#define LIBFLATSURF_TEST_GENERATORS_DEFORMATION_GENERATORS_HPP

#include "../../flatsurf/deformation.hpp"

#include "../external/catch2/single_include/catch2/catch.hpp"

namespace flatsurf::test {

template <typename T>
class DeformationGenerator : public Catch::Generators::IGenerator<Deformation<FlatTriangulation<T>>> {
  std::shared_ptr<const FlatTriangulation<T>> surface;

  Deformation<FlatTriangulation<T>> current;

 public:
  DeformationGenerator(std::shared_ptr<const FlatTriangulation<T>> surface) : surface(surface), current(*surface) {
    // Currently, the only deformation we support is a lengthy sequence of flips.
    auto domain = surface->clone();
    auto deformed = domain.clone();
    auto track = Tracked(deformed, Deformation(*surface));

    for (auto edge : deformed.edges()) {
      if (deformed.convex(edge.positive(), true))
        deformed.flip(edge.positive());
    }

    deformed.delaunay();

    current = *track;
  }

  const Deformation<FlatTriangulation<T>>& get() const override {
    return current;
  }

  bool next() override {
    return false;
  }
};

template <typename T>
Catch::Generators::GeneratorWrapper<Deformation<FlatTriangulation<T>>> deformations(std::shared_ptr<const FlatTriangulation<T>> surface) {
  return Catch::Generators::GeneratorWrapper<Deformation<FlatTriangulation<T>>>(std::unique_ptr<Catch::Generators::IGenerator<Deformation<FlatTriangulation<T>>>>(new DeformationGenerator<T>(surface)));
}

}

#endif
