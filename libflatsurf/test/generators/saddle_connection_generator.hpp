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

#ifndef LIBFLATSURF_TEST_GENERATORS_SADDLE_CONNECTION_GENERATOR_HPP
#define LIBFLATSURF_TEST_GENERATORS_SADDLE_CONNECTION_GENERATOR_HPP

#include <memory>

#include "../../flatsurf/bound.hpp"
#include "../../flatsurf/saddle_connection.hpp"
#include "../../flatsurf/saddle_connections.hpp"
#include "../../flatsurf/saddle_connections_by_length.hpp"
#include "../../flatsurf/saddle_connections_by_length_iterator.hpp"
#include "../../flatsurf/saddle_connections_iterator.hpp"
#include "surface_generator.hpp"
#include "../external/catch2/single_include/catch2/catch.hpp"

namespace flatsurf::test {

using std::begin;
using std::end;

template <typename T, typename C>
class SaddleConnectionGenerator : public Catch::Generators::IGenerator<SaddleConnection<FlatTriangulation<T>>> {
  C connections;
  typename C::iterator current;
  int count;
  const int skip;

 public:
  SaddleConnectionGenerator(const C& connections, int count = -1, int skip = 0) :
    connections(connections),
    current(begin(this->connections)),
    count(count),
    skip(skip) {}

  const SaddleConnection<FlatTriangulation<T>>& get() const override {
    return *current;
  }

  bool next() override {
    for (int i = 0; i < skip && current != end(connections); i++)
      ++current;

    if (count == 0)
      return false;

    if (current == end(connections))
      return false;

    count--;

    return true;
  }
};

template <typename T>
Catch::Generators::GeneratorWrapper<SaddleConnection<FlatTriangulation<T>>> saddleConnections(const NamedSurface<T>& surface, Bound bound) {
  return Catch::Generators::GeneratorWrapper<SaddleConnection<FlatTriangulation<T>>>(std::unique_ptr<Catch::Generators::IGenerator<SaddleConnection<FlatTriangulation<T>>>>(new SaddleConnectionGenerator<T, SaddleConnection<FlatTriangulation<T>>>(surface->connections().bound(bound))));
}

// Generates a sample of count saddle connections on this surface.
// If count is not given, roughly as many saddle connections are created as
// there are edges in the surface.  Note that we skip some connections before
// returning the next one so we get a good sample of very short saddle
// connections, coming from actual edges of the surface and longer connections.
template <typename T>
Catch::Generators::GeneratorWrapper<SaddleConnection<FlatTriangulation<T>>> saddleConnections(const NamedSurface<T>& surface, int count = -1, int skip = -1) {
  if (count == -1)
    count = static_cast<int>(surface->size());
  if (skip == -1)
    skip = static_cast<int>(surface->size()) * 3 / count;

  return Catch::Generators::GeneratorWrapper<SaddleConnection<FlatTriangulation<T>>>(std::unique_ptr<Catch::Generators::IGenerator<SaddleConnection<FlatTriangulation<T>>>>(new SaddleConnectionGenerator<T, SaddleConnectionsByLength<FlatTriangulation<T>>>(surface->connections().byLength(), count, skip)));
}

template <typename T>
Catch::Generators::GeneratorWrapper<SaddleConnection<FlatTriangulation<T>>> saddleConnections(const NamedSurface<T>& surface, const Vertex& source, int count = -1, int skip = -1) {
  if (count == -1)
    count = static_cast<int>(surface->size());
  if (skip == -1)
    skip = static_cast<int>(surface->size()) * 3 / count;

  return Catch::Generators::GeneratorWrapper<SaddleConnection<FlatTriangulation<T>>>(std::unique_ptr<Catch::Generators::IGenerator<SaddleConnection<FlatTriangulation<T>>>>(new SaddleConnectionGenerator<T, SaddleConnectionsByLength<FlatTriangulation<T>>>(surface->connections().byLength().source(source), count, skip)));
}

}  // namespace flatsurf::test

#endif
