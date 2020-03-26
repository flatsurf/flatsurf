/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020 Vincent Delecroix
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

#ifndef LIBFLATSURF_TEST_GENERATORS_COMBINATORIAL_SURFACE_GENERATORS_HPP
#define LIBFLATSURF_TEST_GENERATORS_COMBINATORIAL_SURFACE_GENERATORS_HPP

#include <memory>

#include <e-antic/renf_elem.h>
#include <gmpxx.h>

#include "../external/catch2/single_include/catch2/catch.hpp"

#include "../../flatsurf/forward.hpp"

#include "../surfaces.hpp"

namespace flatsurf::test {

class CombinatorialSurfaceGenerator : public Catch::Generators::IGenerator<std::shared_ptr<FlatTriangulationCombinatorial>> {
  enum class Surface {
    SQUARE,
    L,
    _125,
    _1221,
    HEXAGON,
    HEPTAGON_L,
    LAST = HEPTAGON_L
  };

  Surface current;
  std::shared_ptr<FlatTriangulationCombinatorial> value;

  static std::shared_ptr<FlatTriangulationCombinatorial> make(Surface surface) {
    switch (surface) {
      case Surface::SQUARE:
        return makeSquareCombinatorial();
      case Surface::L:
        return makeLCombinatorial();
      case Surface::_125:
        return make125Combinatorial();
      case Surface::_1221:
        return make1221Combinatorial();
      case Surface::HEXAGON:
        return makeHexagonCombinatorial();
      case Surface::HEPTAGON_L:
        return makeHeptagonLCombinatorial();
      default:
        throw std::logic_error("unknown Surface type");
    }
  }

 public:
  CombinatorialSurfaceGenerator() :
    current(Surface::SQUARE),
    value(make(current)) {}

  const std::shared_ptr<FlatTriangulationCombinatorial>& get() const override {
    return value;
  }

  bool next() override {
    if (current == Surface::LAST) return false;

    current = static_cast<Surface>(static_cast<int>(current) + 1);

    value = make(current);

    if (value == nullptr)
      return next();

    return true;
  }
};

Catch::Generators::GeneratorWrapper<std::shared_ptr<FlatTriangulationCombinatorial>> makeSurfaceCombinatorial() {
  return Catch::Generators::GeneratorWrapper<std::shared_ptr<FlatTriangulationCombinatorial>>(std::unique_ptr<Catch::Generators::IGenerator<std::shared_ptr<FlatTriangulationCombinatorial>>>(new CombinatorialSurfaceGenerator()));
}

}  // namespace flatsurf::test

#endif
