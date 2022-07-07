/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C)      2020 Vincent Delecroix
 *        Copyright (C) 2020-2022 Julian Rüth
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
class SurfaceGenerator : public Catch::Generators::IGenerator<std::tuple<std::string, std::shared_ptr<const FlatTriangulation<T>>>> {
  using Surfaces = std::vector<std::tuple<std::string, std::shared_ptr<const FlatTriangulation<T>>>>;

  Surfaces surfaces;

  typename Surfaces::const_iterator current;

 public:
  SurfaceGenerator() {
    using R2 = Vector<T>;

    surfaces.push_back({"Torus", makeSquare<R2>()});
    surfaces.push_back({"Rectangle", makeRectangle<R2>()});
    surfaces.push_back({"L", makeL<R2>()});
    surfaces.push_back({"McMullen L 1114", makeMcMullenL1114<R2>()});
    surfaces.push_back({"McMullen L 2111", makeMcMullenL2111<R2>()});
    surfaces.push_back({"McMullen L 3125", makeMcMullenL3125<R2>()});
    if constexpr (hasFractions<T>)
      surfaces.push_back({"Cathedral", makeCathedral<R2>()});
    if constexpr (hasFractions<T> && hasNumberFieldElements<T>)
      surfaces.push_back({"Cathedral Quartic", makeCathedralQuartic<R2>()});
    if constexpr (hasFractions<T> && hasNumberFieldElements<T>)
      surfaces.push_back({"Cathedral Veech", makeCathedralVeech<R2>()});
    if constexpr (hasNumberFieldElements<T>)
      surfaces.push_back({"McMullen Genus 2", makeMcMullenGenus2<R2>()});
    if constexpr (hasNumberFieldElements<T>)
      surfaces.push_back({"(1, 2, 3)", make123<R2>()});
    if constexpr (hasFractions<T> && hasNumberFieldElements<T>)
      surfaces.push_back({"(1, 2, 5)", make125<R2>()});
    if constexpr (hasFractions<T> && hasNumberFieldElements<T>)
      surfaces.push_back({"(1, 2, 2, 1)", make1221<R2>()});
    if constexpr (hasFractions<T> && hasNumberFieldElements<T>)
      surfaces.push_back({"(1, 2, 3, 4)", make1234<R2>()});
    if constexpr (hasNumberFieldElements<T>)
      surfaces.push_back({"(2, 2, 3, 13)", make22313<R2>()});
    if constexpr (hasFractions<T> && hasNumberFieldElements<T>)
      surfaces.push_back({"(2, 3, 5)", make235<R2>()});
    if constexpr (hasFractions<T> && hasNumberFieldElements<T>)
      surfaces.push_back({"Equilateral Hexagon", makeHexagon<R2>()});
    if constexpr (hasFractions<T> && hasNumberFieldElements<T>)
      surfaces.push_back({"Equilateral Octagon", makeOctagon<R2>()});
    if constexpr (hasFractions<T> && hasNumberFieldElements<T>)
      surfaces.push_back({"Heptagon L", makeHeptagonL<R2>()});
    if constexpr (hasNumberFieldElements<T>)
      surfaces.push_back({"Golden L", makeGoldenL<R2>()});
    if constexpr (hasNumberFieldElements<T>)
      surfaces.push_back({"(10, 7, 2, 2, 2, 1)", make1072221<R2>()});
    if constexpr (hasFractions<T>)
      surfaces.push_back({"ThurstonVeech((1, 2, 3, 4, 5, 0), (3, 4, 0, 1, 5, 2), [3], [3])", make12345034015233<R2>()});

    current = surfaces.begin();
  }

  const std::tuple<std::string, std::shared_ptr<const FlatTriangulation<T>>>& get() const override {
    return *current;
  }

  bool next() override {
    current++;

    return current != surfaces.end();
  }
};

#define GENERATE_SURFACES(T) \
  []() { \
    const auto [name, surface_] = GENERATE(surfaces<T>()); \
    CAPTURE(name); \
    CAPTURE(*surface_); \
    return surface_; }()
  

template <typename T>
Catch::Generators::GeneratorWrapper<std::tuple<std::string, std::shared_ptr<const FlatTriangulation<T>>>> surfaces() {
  return Catch::Generators::GeneratorWrapper<std::tuple<std::string, std::shared_ptr<const FlatTriangulation<T>>>>(std::unique_ptr<Catch::Generators::IGenerator<std::tuple<std::string, std::shared_ptr<const FlatTriangulation<T>>>>>(new SurfaceGenerator<T>()));
}

// TODO: Delete
template <typename T>
class DeletemeSurfaceGenerator : public Catch::Generators::IGenerator<std::tuple<std::string*, std::shared_ptr<FlatTriangulation<T>>*>> {
  enum class Surface {
    SQUARE,
    RECTANGLE,
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
    _22313,
    _235,
    HEXAGON,
    OCTAGON,
    HEPTAGON_L,
    GOLDEN_L,
    _1072221,
    _12345034015233,
    LAST = _12345034015233,
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
      case Surface::RECTANGLE:
        name = "Rectangle";
        value = makeRectangle<R2>();
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
      case Surface::_22313:
        if constexpr (hasNumberFieldElements<T>) {
          name = "(2, 2, 3, 13)";
          value = make22313<R2>();
        } else
          value = nullptr;
        return;
      case Surface::_235:
        if constexpr (hasFractions<T> && hasNumberFieldElements<T>) {
          name = "(2, 3, 5)";
          value = make235<R2>();
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
      case Surface::_1072221:
        if constexpr (hasNumberFieldElements<T>) {
          name = "(10, 7, 2, 2, 2, 1)";
          value = make1072221<R2>();
        } else
          value = nullptr;
        return;
      case Surface::_12345034015233:
        if constexpr (hasFractions<T>) {
          name = "ThurstonVeech((1, 2, 3, 4, 5, 0), (3, 4, 0, 1, 5, 2), [3], [3])";
          value = make12345034015233<R2>();
        } else
          value = nullptr;
        return;
      default:
        throw std::logic_error("unknown Surface type");
    }
  }

 public:
  DeletemeSurfaceGenerator() :
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
  return Catch::Generators::GeneratorWrapper<std::tuple<std::string*, std::shared_ptr<FlatTriangulation<T>>*>>(std::unique_ptr<Catch::Generators::IGenerator<std::tuple<std::string*, std::shared_ptr<FlatTriangulation<T>>*>>>(new DeletemeSurfaceGenerator<T>()));
}

}  // namespace flatsurf::test

#endif
