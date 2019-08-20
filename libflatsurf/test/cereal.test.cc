/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Vincent Delecroix
 *        Copyright (C) 2019 Julian Rüth
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

#include <gtest/gtest.h>
#include <boost/lexical_cast.hpp>
#include <cereal/archives/json.hpp>

#include <flatsurf/cereal.hpp>
#include <intervalxt/length.hpp>

#include "surfaces.hpp"

using namespace flatsurf;
using cereal::JSONInputArchive;
using cereal::JSONOutputArchive;
using eantic::renf_class;
using std::vector;
using testing::Test;
using testing::Types;

namespace flatsurf {

template <typename T>
struct is_shared_ptr : std::false_type {};
template <typename T>
struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};

template <typename T>
struct factory {
  static std::unique_ptr<T> make() { return std::make_unique<T>(); }
};

template <>
struct factory<Vertex> {
  static std::unique_ptr<Vertex> make() {
    auto parent = FlatTriangulationCombinatorial({{1, -1}});
    return std::make_unique<Vertex>(Vertex::source(HalfEdge(1), parent));
  }
};

template <typename Surface>
struct factory<SaddleConnection<Surface>> {
  static std::unique_ptr<SaddleConnection<Surface>> make() {
    auto square = makeSquare<typename Surface::Vector>();
    return *SaddleConnections<Surface>(square, Bound(8)).begin();
  }
};

template <typename T>
struct printer {
  static std::string toString(const T& x) { return boost::lexical_cast<std::string>(x); }
};

template <typename T>
struct printer<std::shared_ptr<T>> {
  static std::string toString(const std::shared_ptr<T>& x) {
    if (x)
      return boost::lexical_cast<std::string>(x) + " -> " + printer<T>::toString(*x);
    else
      return "null";
  }
};

template <typename T>
struct comparer {
  static bool eq(const T& x, const T& y) { return x == y; }
};

template <typename T>
struct comparer<std::shared_ptr<T>> {
  static bool eq(const std::shared_ptr<T>& x, const std::shared_ptr<T>& y) { return *x == *y; }
};

template <typename Surface>
struct comparer<SaddleConnection<Surface>> {
  static bool eq(const SaddleConnection<Surface>& x, const SaddleConnection<Surface>& y) {
    return x.surface() == y.surface() && x.source() == y.source() && x.target() == y.target() && x.vector() == y.vector();
  }
};

template <typename T>
std::unique_ptr<T> test_serialization(const T& x) {
  std::stringstream s;

  {
    JSONOutputArchive archive(s);
    archive(cereal::make_nvp("test", x));
  }

  auto y = factory<T>::make();

  {
    JSONInputArchive archive(s);
    archive(cereal::make_nvp("test", *y));
  }

  if (!comparer<T>::eq(x, *y)) {
    throw std::runtime_error("deserialization failed to reconstruct element, the original value " + printer<T>::toString(x) + " had serialized to " + s.str() + " which deserialized to " + printer<T>::toString(*y));
  }

  return y;
}

TEST(CerealTest, HalfEdge) {
  test_serialization(HalfEdge(1337));
}

TEST(CerealTest, Vertex) {
  auto square = makeSquare<Vector<long long>>();
  test_serialization(square->vertices()[0]);
}

TEST(CerealTest, FlatTriangulationCombinatorial) {
  auto vertices = vector<vector<int>>{{1, 3, 2, -1, -3, -2}};
  auto square = std::make_shared<FlatTriangulationCombinatorial>(vertices);
  test_serialization(square);
}

TEST(CerealTest, FlatTriangulation) {
  test_serialization(*makeSquare<Vector<long long>>());
}

TEST(CerealTest, SaddleConnection) {
  auto square = makeSquare<Vector<long long>>();
  auto sc = SaddleConnections(square, Bound(8));
  test_serialization(**sc.begin());
}

}  // namespace flatsurf

#include "main.hpp"
