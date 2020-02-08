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

#ifndef LIBFLATSURF_TEST_GENERATORS_SADDLE_CONNECTION_GENERATORS_HPP
#define LIBFLATSURF_TEST_GENERATORS_SADDLE_CONNECTION_GENERATORS_HPP

#include <memory>

#include <gmpxx.h>
#include <e-antic/renf_elem.h>

#include "../external/catch2/single_include/catch2/catch.hpp"

#include "../../flatsurf/saddle_connections.hpp"
#include "../../flatsurf/bound.hpp"

namespace flatsurf::test {

template <typename T>
class SaddleConnectionsGenerator : public Catch::Generators::IGenerator<SaddleConnection<FlatTriangulation<T>>> {
  SaddleConnections<FlatTriangulation<T>> connections;
  typename SaddleConnections<FlatTriangulation<T>>::Iterator current;

public:
  SaddleConnectionsGenerator(std::shared_ptr<FlatTriangulation<T>> surface, Bound bound=Bound(8, 0)) :
    connections(surface, bound),
    current(begin(connections)) {}

  const SaddleConnection<FlatTriangulation<T>>& get() const override {
    return *current;
  }

  bool next() override {
    current++;

    return current != end(connections);
  }
};

template <typename T>
Catch::Generators::GeneratorWrapper<SaddleConnection<FlatTriangulation<T>>> saddleConnections(std::shared_ptr<FlatTriangulation<T>> surface) {
  return Catch::Generators::GeneratorWrapper<SaddleConnection<FlatTriangulation<T>>>(std::unique_ptr<Catch::Generators::IGenerator<SaddleConnection<FlatTriangulation<T>>>>(new SaddleConnectionsGenerator<T>(surface)));
}

}

#endif

