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

namespace flatsurf::test {
namespace {

template <typename T>
struct factory {
  static std::unique_ptr<T> make() { return std::make_unique<T>(); }
};

template <>
struct factory<Vertex> {
  static std::unique_ptr<Vertex> make() {
    auto parent = makeSquareCombinatorial();
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

}  // namespace
}  // namespace flatsurf::test
