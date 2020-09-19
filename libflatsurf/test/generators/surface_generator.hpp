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

#ifndef LIBFLATSURF_TEST_GENERATORS_SURFACE_GENERATORS_HPP
#define LIBFLATSURF_TEST_GENERATORS_SURFACE_GENERATORS_HPP

#include <e-antic/renf_elem.h>
#include <gmpxx.h>

#include <memory>

#include "../../flatsurf/forward.hpp"
#include "../external/catch2/single_include/catch2/catch.hpp"
#include "../surfaces.hpp"
#include "real_generator.hpp"

namespace flatsurf::test {

template <typename T>
class SurfaceGenerator : public Catch::Generators::IGenerator<std::tuple<std::string*, std::shared_ptr<FlatTriangulation<T>>*>> {
  enum class Surface {
    SQUARE,
    L,
    MCMULLEN_L1114,
    MCMULLEN_L2111,
    MCMULLEN_L3125,
    CATHEDRAL,
    CATHEDRAL_QUARTIC,
    CATHEDRAL_VEECH,
    MCMULLEN_GENUS2,
    _123,
    _125,
    _1221,
    _1234,
    HEXAGON,
    OCTAGON,
    HEPTAGON_L,
    GOLDEN_L,
    LAST = GOLDEN_L,
  };

  Surface current;
  std::shared_ptr<FlatTriangulation<T>> value;
  std::string name;

  std::tuple<std::string*, std::shared_ptr<FlatTriangulation<T>>*> gettable;

  using R2 = Vector<T>;

  void make(Surface surface) {
    switch (surface) {
      case Surface::SQUARE:
        name = "Torus";
        value = makeSquare<R2>();
        return;
      case Surface::L:
        name = "L";
        value = makeL<R2>();
        return;
      case Surface::MCMULLEN_L1114:
        name = "McMullen L 1114";
        value = makeMcMullenL1114<R2>();
        return;
      case Surface::MCMULLEN_L2111:
        name = "McMullen L 2111";
        value = makeMcMullenL2111<R2>();
        return;
      case Surface::MCMULLEN_L3125:
        name = "McMullen L 3125";
        value = makeMcMullenL3125<R2>();
        return;
      case Surface::CATHEDRAL:
        if constexpr (hasFractions<T>) {
          name = "Cathedral";
          value = makeCathedral<R2>();
        } else
          value = nullptr;
        return;
      case Surface::CATHEDRAL_QUARTIC:
        if constexpr (hasFractions<T> && hasNumberFieldElements<T>) {
          name = "Cathedral Quartic";
          value = makeCathedralQuartic<R2>();
        } else
          value = nullptr;
        return;
      case Surface::CATHEDRAL_VEECH:
        if constexpr (hasFractions<T> && hasNumberFieldElements<T>) {
          name = "Cathedral Veech";
          value = makeCathedralVeech<R2>();
        } else
          value = nullptr;
        return;
      case Surface::MCMULLEN_GENUS2:
        if constexpr (hasNumberFieldElements<T>) {
          name = "McMullen Genus 2";
          value = makeMcMullenGenus2<R2>();
        } else
          value = nullptr;
        return;
      case Surface::_123:
        if constexpr (hasNumberFieldElements<T>) {
          name = "(1, 2, 3)";
          value = make123<R2>();
        } else
          value = nullptr;
        return;
      case Surface::_125:
        if constexpr (hasFractions<T> && hasNumberFieldElements<T>) {
          name = "(1, 2, 5)";
          value = make125<R2>();
        } else
          value = nullptr;
        return;
      case Surface::_1221:
        if constexpr (hasFractions<T> && hasNumberFieldElements<T>) {
          name = "(1, 2, 2, 1)";
          value = make1221<R2>();
        } else
          value = nullptr;
        return;
      case Surface::_1234:
        if constexpr (hasFractions<T> && hasNumberFieldElements<T>) {
          name = "(1, 2, 3, 4)";
          value = make1234<R2>();
        } else
          value = nullptr;
        return;
      case Surface::HEXAGON:
        if constexpr (hasFractions<T> && hasNumberFieldElements<T>) {
          name = "Equilateral Hexagon";
          value = makeHexagon<R2>();
        } else
          value = nullptr;
        return;
      case Surface::OCTAGON:
        if constexpr (hasFractions<T> && hasNumberFieldElements<T>) {
          name = "Equilateral Octagon";
          value = makeOctagon<R2>();
        } else
          value = nullptr;
        return;
      case Surface::HEPTAGON_L:
        if constexpr (hasFractions<T> && hasNumberFieldElements<T>) {
          name = "Heptagon L";
          value = makeHeptagonL<R2>();
        } else
          value = nullptr;
        return;
      case Surface::GOLDEN_L:
        if constexpr (hasNumberFieldElements<T>) {
          name = "Golden L";
          value = makeGoldenL<R2>();
        } else
          value = nullptr;
        return;
      default:
        throw std::logic_error("unknown Surface type");
    }
  }

 public:
  SurfaceGenerator() :
    current(Surface::SQUARE),
    gettable({&name, &value}) {
    make(current);
  }

  const std::tuple<std::string*, std::shared_ptr<FlatTriangulation<T>>*>& get() const override {
    return gettable;
  }

  bool next() override {
    if (current == Surface::LAST) return false;

    current = static_cast<Surface>(static_cast<int>(current) + 1);

    make(current);

    if (value == nullptr)
      return next();

    return true;
  }
};

template <typename T>
Catch::Generators::GeneratorWrapper<std::tuple<std::string*, std::shared_ptr<FlatTriangulation<T>>*>> makeSurface() {
  return Catch::Generators::GeneratorWrapper<std::tuple<std::string*, std::shared_ptr<FlatTriangulation<T>>*>>(std::unique_ptr<Catch::Generators::IGenerator<std::tuple<std::string*, std::shared_ptr<FlatTriangulation<T>>*>>>(new SurfaceGenerator<T>()));
}

}  // namespace flatsurf::test

#endif
