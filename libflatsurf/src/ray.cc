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

#include "../flatsurf/ray.hpp"

#include "impl/ray.impl.hpp"

#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/point.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/segment.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/vertical.hpp"

namespace flatsurf {
template <typename Surface>
Ray<Surface>::Ray(const Point<Surface>&, const Vector<T>&) {
  throw std::logic_error("not implemented: Ray()");
}

template <typename Surface>
Ray<Surface>::Ray(const Point<Surface>&, HalfEdge source, const Vector<T>&) {
  throw std::logic_error("not implemented: Ray()");
}

template <typename Surface>
Ray<Surface>::Ray(const Surface&, HalfEdge source, const Vector<T>&) {
  throw std::logic_error("not implemented: Ray()");
}

template <typename Surface>
Ray<Surface>::Ray(const Surface&, HalfEdge) {
  throw std::logic_error("not implemented: Ray()");
}

template <typename Surface>
const Point<Surface>& Ray<Surface>::start() const {
  throw std::logic_error("not implemented: start()");
}

template <typename Surface>
HalfEdge Ray<Surface>::source() const {
  throw std::logic_error("not implemented: source()");
}

template <typename Surface>
Ray<Surface>::operator const Vector<T> &() const {
  throw std::logic_error("not implemented: operator Vector()");
}

template <typename Surface>
const Vector<typename Surface::Coordinate>& Ray<Surface>::vector() const {
  throw std::logic_error("not implemented: vector()");
}

template <typename Surface>
Ray<Surface>::operator Vertical<Surface>() const {
  throw std::logic_error("not implemented: operator Vertical()");
}

template <typename Surface>
SaddleConnection<Surface> Ray<Surface>::saddleConnection() const {
  throw std::logic_error("not implemented: saddleConnection()");
}

template <typename Surface>
int Ray<Surface>::angle(const Ray&) const {
  throw std::logic_error("not implemented: angle()");
}

template <typename Surface>
CCW Ray<Surface>::ccw(const Ray&) const {
  throw std::logic_error("not implemented: ccw()");
}

template <typename Surface>
bool Ray<Surface>::operator==(const Ray&) const {
  throw std::logic_error("not implemented: operator==()");
}

template <typename Surface>
const Surface& Ray<Surface>::surface() const {
  throw std::logic_error("not implemented: surface()");
}

template <typename Surface>
Segment<Surface> Ray<Surface>::segment(const Point<Surface>& end) const {
  throw std::logic_error("not implemented: segment()");
}

template <typename Surface>
std::ostream &operator<<(std::ostream &, const Ray<Surface> &) {
  throw std::logic_error("not implemented: printing of rays");
}

}

namespace std {

using namespace flatsurf;

template <typename Surface>
size_t hash<Ray<Surface>>::operator()(const Ray<Surface>& self) const {
  throw std::logic_error("not implemented: hashing of rays");
}

}

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION)(LIBFLATSURF_INSTANTIATE_HASH), Ray, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
