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

#ifndef LIBFLATSURF_TEST_GENERATORS_VERTICAL_GENERATORS_HPP
#define LIBFLATSURF_TEST_GENERATORS_VERTICAL_GENERATORS_HPP

#include <memory>

#include "../external/catch2/single_include/catch2/catch.hpp"

#include "../../flatsurf/bound.hpp"
#include "../../flatsurf/saddle_connection.hpp"
#include "../../flatsurf/saddle_connections.hpp"
#include "../../flatsurf/saddle_connections_iterator.hpp"
#include "../../flatsurf/vector.hpp"

namespace flatsurf::test {

// Generates verticals coming from saddle connections up to a certain length.
// This differs from the saddle connections generator in that it skips over
// negatives of previous directions and also does not report the same direction
// starting at a different half edge.
template <typename T>
class VerticalGenerator : public Catch::Generators::IGenerator<Vector<T>> {
  std::unordered_set<Vector<T>> verticals;
  SaddleConnections<FlatTriangulation<T>> connections;
  typename SaddleConnections<FlatTriangulation<T>>::Iterator upcoming;
  Vector<T> current;

 public:
  VerticalGenerator(std::shared_ptr<FlatTriangulation<T>> surface, Bound bound = Bound(3, 0)) :
    verticals{},
    connections(surface, bound),
    upcoming(begin(connections)) {
    next();
  }

  const Vector<T>& get() const override {
    return current;
  }

  bool next() override {
    current = *upcoming;

    do {
      ++upcoming;
      if (upcoming == end(connections))
        return false;
    } while (verticals.find(*upcoming) != end(verticals) || verticals.find(-*upcoming) != end(verticals));

    verticals.insert(*upcoming);

    return true;
  }
};

template <typename T>
Catch::Generators::GeneratorWrapper<Vector<T>> verticals(std::shared_ptr<FlatTriangulation<T>> surface) {
  return Catch::Generators::GeneratorWrapper<Vector<T>>(std::unique_ptr<Catch::Generators::IGenerator<Vector<T>>>(new VerticalGenerator<T>(surface)));
}

}  // namespace flatsurf::test

#endif
