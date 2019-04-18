/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
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

#include "flatsurf/cppyy.hpp"

#include <ostream>

using namespace flatsurf;

namespace flatsurf {
std::ostream& operator<<(std::ostream& os, const HalfEdge& self);
std::ostream& operator<<(std::ostream& os, const VectorLongLong& self);
std::ostream& operator<<(std::ostream& os, const VectorEAntic& self);
std::ostream& operator<<(std::ostream& os, const VectorArb& self);
template <typename T>
std::ostream& operator<<(std::ostream& os, const Permutation<T>& self);
template <typename T>
std::ostream& operator<<(std::ostream& os, const HalfEdgeMap<T>& self);
template <typename T>
std::ostream& operator<<(std::ostream& os, const FlatTriangulation<T>& self);
}  // namespace flatsurf

template <typename T>
std::ostream& print(std::ostream& os, const T& self) {
  return flatsurf::operator<<(os, self);
}

std::ostream& operator<<(std::ostream& os, const HalfEdge& self) {
  return print(os, self);
}
std::ostream& operator<<(std::ostream& os, const VectorLongLong& self) {
  return print(os, self);
}
std::ostream& operator<<(std::ostream& os, const VectorEAntic& self) {
  return print(os, self);
}
std::ostream& operator<<(std::ostream& os, const VectorArb& self) {
  return print(os, self);
}
std::ostream& operator<<(std::ostream& os, const Permutation<HalfEdge>& self) {
  return print(os, self);
}
std::ostream& operator<<(std::ostream& os,
                         const HalfEdgeMap<VectorLongLong>& self) {
  return print(os, self);
}
std::ostream& operator<<(std::ostream& os,
                         const HalfEdgeMap<VectorEAntic>& self) {
  return print(os, self);
}
std::ostream& operator<<(std::ostream& os, const HalfEdgeMap<int>& self) {
  return print(os, self);
}
std::ostream& operator<<(std::ostream& os,
                         const FlatTriangulation<VectorLongLong>& self) {
  return print(os, self);
}
std::ostream& operator<<(std::ostream& os,
                         const FlatTriangulation<VectorEAntic>& self) {
  return print(os, self);
}
