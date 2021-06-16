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

#ifndef LIBFLATSURF_TEST_CEREAL_HELPERS_HPP
#define LIBFLATSURF_TEST_CEREAL_HELPERS_HPP

#include <exact-real/arb.hpp>
#include <memory>
#include <boost/lexical_cast.hpp>

#include "../flatsurf/edge.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/vertex.hpp"
#include "./surfaces.hpp"

namespace flatsurf::test {

template <typename T>
struct factory {
  static std::unique_ptr<T> make() { return std::make_unique<T>(); }
};

template <>
struct factory<Vertex> {
  static std::unique_ptr<Vertex> make() {
    auto parent = makeSquareCombinatorial();
    return std::make_unique<Vertex>(Vertex::source(HalfEdge(1), *parent));
  }
};

template <typename Surface>
struct factory<Chain<Surface>> {
  using T = typename Surface::Coordinate;

  static std::shared_ptr<Chain<Surface>> make() {
    auto square = makeSquare<Vector<T>>();
    return std::make_shared<Chain<Surface>>(*square);
  }
};

template <typename Surface>
struct factory<SaddleConnection<Surface>> {
  using T = typename Surface::Coordinate;

  static std::shared_ptr<SaddleConnection<Surface>> make() {
    auto square = makeSquare<Vector<T>>();
    const HalfEdge e(1);
    return std::make_shared<SaddleConnection<Surface>>(*square, e);
  }
};

template <typename Surface>
struct factory<Vertical<Surface>> {
  using T = typename Surface::Coordinate;

  static std::shared_ptr<Vertical<Surface>> make() {
    auto square = makeSquare<Vector<T>>();
    auto vertical = square->fromHalfEdge(HalfEdge(1));
    return std::make_shared<Vertical<Surface>>(*square, vertical);
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

template <>
struct comparer<exactreal::Arb> {
  static bool eq(const exactreal::Arb& x, const exactreal::Arb& y) {
    return x.equal(y);
  }
};

template <>
struct comparer<flatsurf::Vector<exactreal::Arb>> {
  static bool eq(const flatsurf::Vector<exactreal::Arb>& x, const flatsurf::Vector<exactreal::Arb>& y) {
    std::stringstream a, b;
    a << x;
    b << y;
    return a.str() == b.str();
  }
};

template <typename Surface>
struct comparer<flatsurf::Vertical<Surface>> {
  static bool eq(const flatsurf::Vertical<Surface>& x, const flatsurf::Vertical<Surface>& y) {
    // Vertical::operator== requires the surfaces to be identical to consider
    // two vertical equal.
    return x.surface() == y.surface() && x.vertical() == y.vertical();
  }
};

}  // namespace flatsurf::test

#endif
