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


// A wrapper of a pair (name, surface) so that we can conveniently GENERATE
// surfaces while keeping track of their name in Catch2.
template <typename T>
struct NamedSurface {
  using Surface = FlatTriangulation<T>;

  NamedSurface(const std::shared_ptr<Surface>& surface) : NamedSurface(std::tuple{"?", surface}) {}

  NamedSurface(const std::shared_ptr<const Surface>& surface) : NamedSurface(std::tuple{"?", surface}) {}

  NamedSurface(std::tuple<std::string, std::shared_ptr<const Surface>> surface) : surface(std::get<1>(surface)), name(std::get<0>(surface)) {}

  operator const std::shared_ptr<const Surface>&() const { return surface; }
  operator const std::string&() const { return name; }

  friend
  std::ostream& operator<<(std::ostream& os, const NamedSurface& surface) {
    return os << surface.name;
  }

  const FlatTriangulation<T>* operator->() const { return surface.get(); }
  const FlatTriangulation<T>& operator*() const { return *surface; }

  std::shared_ptr<const FlatTriangulation<T>> surface;
  std::string name;
};

template <typename T>
NamedSurface(std::shared_ptr<FlatTriangulation<T>>) -> NamedSurface<T>;

template <typename T>
NamedSurface(std::shared_ptr<const FlatTriangulation<T>>) -> NamedSurface<T>;


#define GENERATE_SURFACES(T) \
  NamedSurface<T>(GENERATE(::flatsurf::test::surfaces<T>()))


template <typename T>
Catch::Generators::GeneratorWrapper<std::tuple<std::string, std::shared_ptr<const FlatTriangulation<T>>>> surfaces() {
  return Catch::Generators::GeneratorWrapper<std::tuple<std::string, std::shared_ptr<const FlatTriangulation<T>>>>(std::unique_ptr<Catch::Generators::IGenerator<std::tuple<std::string, std::shared_ptr<const FlatTriangulation<T>>>>>(new SurfaceGenerator<T>()));
}

}  // namespace flatsurf::test

#endif
