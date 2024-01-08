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

#ifndef LIBFLATSURF_TEST_GENERATORS_DEFORMATION_GENERATOR_HPP
#define LIBFLATSURF_TEST_GENERATORS_DEFORMATION_GENERATOR_HPP

#include "../../flatsurf/deformation.hpp"
#include "../../flatsurf/edge.hpp"
#include "../../flatsurf/permutation.hpp"
#include "surface_generator.hpp"

#include "../external/catch2/single_include/catch2/catch.hpp"

namespace flatsurf::test {

template <typename T>
class DeformationGenerator : public Catch::Generators::IGenerator<Deformation<FlatTriangulation<T>>> {
  std::shared_ptr<const FlatTriangulation<T>> surface;

  using Cache = std::vector<Deformation<FlatTriangulation<T>>>;

  Cache deformations;
  typename Cache::const_iterator current;

 public:
  DeformationGenerator(std::shared_ptr<const FlatTriangulation<T>> surface) : surface(surface), current(deformations.end()) {
    auto domain = surface->clone();

    {
      // A sequence of flips
      auto deformed = domain.clone();
      auto track = Tracked(deformed, Deformation(*surface));

      for (auto edge : deformed.edges()) {
        if (deformed.convex(edge.positive(), true))
          deformed.flip(edge.positive());
      }
      deformations.push_back(*track);

      // Lengthen the sequence by Delaunay triangulating
      deformed.delaunay();
      deformations.push_back(*track);
    }

    {
      // A linear deformation
      deformations.push_back(domain.applyMatrix(T(1), T(2), T(3), T(4)));
    }

    {
      // A relabeling
      std::unordered_map<HalfEdge, HalfEdge> permutation;
      for (const auto& halfEdge : domain.halfEdges())
        permutation[halfEdge] = halfEdge;

      permutation[1] = -2;
      permutation[-1] = 2;
      permutation[2] = -1;
      permutation[-2] = 1;

      deformations.push_back(domain.relabel(Permutation<HalfEdge>(permutation)));
    }

    current = deformations.begin();
  }

  const Deformation<FlatTriangulation<T>>& get() const override {
    return *current;
  }

  bool next() override {
    current++;

    return current != deformations.end();
  }
};

template <typename T>
Catch::Generators::GeneratorWrapper<Deformation<FlatTriangulation<T>>> deformations(const NamedSurface<T> surface) {
  return Catch::Generators::GeneratorWrapper<Deformation<FlatTriangulation<T>>>(std::unique_ptr<Catch::Generators::IGenerator<Deformation<FlatTriangulation<T>>>>(new DeformationGenerator<T>(surface)));
}

}

#endif
