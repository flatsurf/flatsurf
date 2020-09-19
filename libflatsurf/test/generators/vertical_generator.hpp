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
#include <set>

#include "../../flatsurf/bound.hpp"
#include "../../flatsurf/saddle_connection.hpp"
#include "../../flatsurf/saddle_connections.hpp"
#include "../../flatsurf/saddle_connections_by_length.hpp"
#include "../../flatsurf/saddle_connections_by_length_iterator.hpp"
#include "../../flatsurf/saddle_connections_iterator.hpp"
#include "../../flatsurf/vector.hpp"
#include "../external/catch2/single_include/catch2/catch.hpp"

namespace flatsurf::test {

// Generates verticals coming from saddle connections up to a certain length.
// This differs from the saddle connections generator in that it skips over
// negatives of previous directions and also does not report the same direction
// starting at a different half edge.
template <typename T, typename C>
class VerticalGenerator : public Catch::Generators::IGenerator<Vector<T>> {
  std::set<Vector<T>, typename Vector<T>::CompareSlope> verticals;
  C connections;
  typename C::Iterator upcoming;
  Vector<T> current;
  int count;
  const int skip;

 public:
  VerticalGenerator(const C& connections, int count = -1, int skip = 0) :
    verticals{},
    connections(connections),
    upcoming(begin(this->connections)),
    count(count),
    skip(skip) {
    next();
  }

  const Vector<T>& get() const override {
    return current;
  }

  bool next() override {
    current = *upcoming;

    for (int i = 0; i < skip && upcoming != end(connections); i++) {
      do {
        ++upcoming;
        if (upcoming == end(connections))
          return false;
      } while (verticals.find(*upcoming) != end(verticals) || verticals.find(-*upcoming) != end(verticals));
    }

    if (count == 0)
      return false;

    if (upcoming == end(connections))
      return false;

    verticals.insert(*upcoming);

    count--;

    return true;
  }
};

// Generate a sample of count vertical directions on this surface.  If count is
// not given, roughly as many are created as there are edges in the surface.
// Note that we skip some verticals before returning the next one so we get a
// good sample of verticals coming from very short saddle connections, coming
// from actual edges of the surface and longer connections.
template <typename T>
Catch::Generators::GeneratorWrapper<Vector<T>> verticals(std::shared_ptr<FlatTriangulation<T>> surface, int count = -1, int skip = -1) {
  if (count == -1)
    count = static_cast<int>(surface->size());
  if (skip == -1)
    skip = static_cast<int>(surface->size()) * 3 / count;

  return Catch::Generators::GeneratorWrapper<Vector<T>>(std::unique_ptr<Catch::Generators::IGenerator<Vector<T>>>(new VerticalGenerator<T, SaddleConnectionsByLength<FlatTriangulation<T>>>(surface->connections().byLength(), count, skip)));
}

}  // namespace flatsurf::test

#endif
