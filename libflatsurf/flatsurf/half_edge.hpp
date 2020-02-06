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

#ifndef LIBFLATSURF_HALF_EDGE_HPP
#define LIBFLATSURF_HALF_EDGE_HPP

#include <iosfwd>
#include <functional>

#include <stddef.h>
#include <boost/operators.hpp>

#include "forward.hpp"

namespace flatsurf {
// Similar to Edge this is a wrapper to get type-safe HalfEdges without any
// runtime overhead (at least when compiling with -flto.)
class HalfEdge : boost::equality_comparable<HalfEdge> {
 public:
  friend class Edge;
  friend class FlatTriangulationCombinatorial;
  friend class Vertex;

  HalfEdge();
  HalfEdge(const HalfEdge &edge) = default;
  explicit HalfEdge(const int id);

  HalfEdge operator-() const;
  HalfEdge &operator=(const HalfEdge &other);
  bool operator==(const HalfEdge &other) const;
  // TODO: Remove operator< where it makes not mathematical sense.
  bool operator<(const HalfEdge &other) const;
  friend std::ostream &operator<<(std::ostream &, const HalfEdge &);
  Edge edge() const;

  // Return a zero based index for this half edge that can be used to index into an array.
  size_t index() const noexcept;

 private:
  // TODO: It's probably better to store this as its unsigned index().
  // operator- then becomes a bit flip on the last bit.
  int id;

  friend std::hash<HalfEdge>;
  friend cereal::access;
  template <typename Archive>
  int save_minimal(Archive &) const;
  template <typename Archive>
  void load_minimal(Archive &, const int &);
};
}  // namespace flatsurf

namespace std {
template <>
struct hash<flatsurf::HalfEdge> {
  size_t operator()(const flatsurf::HalfEdge &) const noexcept;
};
}  // namespace std

#endif
