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

#ifndef LIBFLATSURF_TEST_GENERATORS_EQUIVALENCE_GENERATOR_HPP
#define LIBFLATSURF_TEST_GENERATORS_EQUIVALENCE_GENERATOR_HPP

#include <e-antic/renf_elem.h>
#include <gmpxx.h>

#include <memory>

#include "../../flatsurf/equivalence.hpp"
#include "../../flatsurf/edge.hpp"
#include "surface_generator.hpp"
#include "../external/catch2/single_include/catch2/catch.hpp"

namespace flatsurf::test {

template <typename T>
class EquivalenceGenerator : public Catch::Generators::IGenerator<Equivalence<FlatTriangulation<T>>> {
  std::shared_ptr<const FlatTriangulation<T>> surface;

  enum class EQUIVALENCE {
    COMBINATORIAL_PRESERVE_ORIENTATION = 0,
    COMBINATORIAL_WITHOUT_ORIENTATION = 1,
    UNLABELED = 2,
    ORTHOGONAL = 3,
    AREA_PRESERVING = 4,
    LINEAR = 5,
    LAST = 5,
  } state;

  Equivalence<FlatTriangulation<T>> current;

  Equivalence<FlatTriangulation<T>> make(EQUIVALENCE kind) {
    switch (kind) {
      case EQUIVALENCE::COMBINATORIAL_PRESERVE_ORIENTATION:
        return Equivalence<FlatTriangulation<T>>::combinatorial(true);
      case EQUIVALENCE::COMBINATORIAL_WITHOUT_ORIENTATION:
        return Equivalence<FlatTriangulation<T>>::combinatorial(false);
      case EQUIVALENCE::UNLABELED:
        return Equivalence<FlatTriangulation<T>>::unlabeled();
      case EQUIVALENCE::ORTHOGONAL:
        return Equivalence<FlatTriangulation<T>>::orthogonal();
      case EQUIVALENCE::AREA_PRESERVING:
        return Equivalence<FlatTriangulation<T>>::areaPreserving();
      case EQUIVALENCE::LINEAR:
        return Equivalence<FlatTriangulation<T>>::linear();
      default:
        throw std::logic_error("not implemented: EquivalenceGenerator::make()");
    }
  }

 public:
  EquivalenceGenerator(std::shared_ptr<const FlatTriangulation<T>> surface) : surface(surface), state(EQUIVALENCE::COMBINATORIAL_PRESERVE_ORIENTATION), current(make(state)) {}

  const Equivalence<FlatTriangulation<T>>& get() const override {
    return current;
  }

  bool next() override {
    if (state == EQUIVALENCE::LAST)
      return false;

    state = static_cast<EQUIVALENCE>(static_cast<int>(state) + 1);

    if (!isField<T>) {
      if (state == EQUIVALENCE::ORTHOGONAL || state == EQUIVALENCE::AREA_PRESERVING || state == EQUIVALENCE::LINEAR)
        return next();
    }

    current = make(state);

    return true;
  }
};

template <typename T>
Catch::Generators::GeneratorWrapper<Equivalence<FlatTriangulation<T>>> equivalences(NamedSurface<T> surface) {
  return Catch::Generators::GeneratorWrapper<Equivalence<FlatTriangulation<T>>>(std::unique_ptr<Catch::Generators::IGenerator<Equivalence<FlatTriangulation<T>>>>(new EquivalenceGenerator<T>(surface)));
}

}

#endif
