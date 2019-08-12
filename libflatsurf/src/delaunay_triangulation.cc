/**********************************************************************
 *  This file is part of flatsurf.
 *
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

#include <ostream>
#include <vector>

#include "flatsurf/delaunay_triangulation.hpp"
#include "flatsurf/flat_triangulation.hpp"
#include "flatsurf/half_edge.hpp"
#include "flatsurf/vector.hpp"

namespace {
template <typename T>
T det(const T& x00, const T& x01, const T& x02, const T& x10, const T& x11, const T& x12, const T& x20, const T& x21, const T& x22) {
  return x00 * (x11 * x22 - x12 * x21) - x10 * (x01 * x22 - x02 * x21) + x20 * (x01 * x12 - x02 * x11);
}
}  // namespace

namespace flatsurf {
template <typename T>
void DelaunayTriangulation<T>::transform(FlatTriangulation<T>& triangulation) {
  bool isDelaunay;
  do {
    isDelaunay = true;
    for (auto edge : triangulation.halfEdges()) {
      if (!test(triangulation, edge)) {
        isDelaunay = false;
        triangulation.flip(edge);
      }
    }
  } while (!isDelaunay);
}

template <typename T>
bool DelaunayTriangulation<T>::test(const FlatTriangulation<T>& triangulation, const HalfEdge edge) {
  // We could eventually use Vector::insideCircumcircle() so vectors can
  // provide optimized implementations of this. However, at the moment, it does
  // not seem worth it.

  // We use the condition described in Wikipedia (whether a certain determinant
  // is positive.) Using the notation there, the face attached to this half
  // edge is the triangle (a, b, c), and the face attached to the reversed half
  // edge is (a, c, d). We use a coordinate system where d=(0,0).
  auto ca = triangulation.fromEdge(edge);
  auto cb = triangulation.fromEdge(triangulation.nextAtVertex(edge));
  auto dc = triangulation.fromEdge(-triangulation.nextInFace(-edge));

  auto a = dc + ca;
  auto b = dc + cb;
  auto c = dc;

  return det(a.x(), a.y(), a.x() * a.x() + a.y() * a.y(), b.x(), b.y(), b.x() * b.x() + b.y() * b.y(), c.x(), c.y(), c.x() * c.x() + c.y() * c.y()) <= 0;
}
}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include <e-antic/renfxx.h>
#include <exact-real/element.hpp>
#include <exact-real/integer_ring.hpp>
#include <exact-real/number_field.hpp>
#include <exact-real/rational_field.hpp>

using namespace flatsurf;

template class flatsurf::DelaunayTriangulation<long long>;
template class flatsurf::DelaunayTriangulation<eantic::renf_elem_class>;
template class flatsurf::DelaunayTriangulation<exactreal::Element<exactreal::IntegerRing>>;
template class flatsurf::DelaunayTriangulation<exactreal::Element<exactreal::RationalField>>;
template class flatsurf::DelaunayTriangulation<exactreal::Element<exactreal::NumberField>>;
