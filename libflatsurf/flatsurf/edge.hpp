/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2020 Julian Rüth
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

#ifndef LIBFLATSURF_EDGE_HPP
#define LIBFLATSURF_EDGE_HPP

#include <boost/operators.hpp>
#include <iosfwd>
#include <vector>

#include "half_edge.hpp"

namespace flatsurf {

class Edge : boost::equality_comparable<Edge> {
 public:
  Edge();
  Edge(int);
  Edge(HalfEdge);

  static Edge fromIndex(size_t);

  HalfEdge positive() const;
  HalfEdge negative() const;

  size_t index() const;

  bool operator==(const Edge &) const;

  friend std::ostream &operator<<(std::ostream &, const Edge &);

 private:
  HalfEdge id;

  friend cereal::access;

  template <typename Archive>
  int save_minimal(Archive &) const;
  template <typename Archive>
  void load_minimal(Archive &, const int &);
};
}  // namespace flatsurf

namespace std {
template <>
struct hash<flatsurf::Edge> {
  size_t operator()(const flatsurf::Edge &) const noexcept;
};

}  // namespace std

#endif
