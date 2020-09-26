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

#ifndef LIBFLATSURF_HALF_EDGE_HPP
#define LIBFLATSURF_HALF_EDGE_HPP

#include <boost/operators.hpp>
#include <functional>
#include <iosfwd>

#include "forward.hpp"

namespace flatsurf {

// A (combinatorial) half edge of a triangulation.
class HalfEdge : boost::equality_comparable<HalfEdge> {
  HalfEdge(PrivateConstructor, size_t idx);

 public:
  HalfEdge() noexcept;
  HalfEdge(int id);
  HalfEdge(const HalfEdge &edge) = default;

  static HalfEdge fromIndex(size_t index);

  HalfEdge operator-() const;

  HalfEdge &operator=(const HalfEdge &other) noexcept;

  bool operator==(const HalfEdge &other) const;

  Edge edge() const;

  // Return a zero based index for this half edge that can be used to index into an array.
  size_t index() const;

  int id() const;

  friend std::ostream &operator<<(std::ostream &, const HalfEdge &);

 private:
  size_t idx;

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
  size_t operator()(const flatsurf::HalfEdge &) const;
};
}  // namespace std

#endif
