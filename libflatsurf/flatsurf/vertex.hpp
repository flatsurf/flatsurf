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

#ifndef LIBFLATSURF_VERTEX_HPP
#define LIBFLATSURF_VERTEX_HPP

#include <boost/operators.hpp>

#include "external/spimpl/spimpl.h"

#include "forward.hpp"

namespace flatsurf {
// In their current implementation, Vertex is a much more bulky object than
// HalfEdge and Edge which are just wrapping a single integer.
class Vertex : boost::equality_comparable<Vertex> {
 public:
  // Construct an invalid vertex.
  Vertex();

  static const Vertex& source(const HalfEdge &, const FlatTriangulationCombinatorial &);
  static const Vertex& target(const HalfEdge &, const FlatTriangulationCombinatorial &);

  // Note that this operator fails to distinguish equally labeled vertices on different surfaces.
  bool operator==(const Vertex &) const;
  bool operator<(const Vertex &) const;

  friend std::ostream &operator<<(std::ostream &, const Vertex &);

  // TODO: Find a better solution for this.
 public:
  // TODO: Can I make this protected in a private base class and use
  // specialization to implement Implementation differently in different
  // classes?
  class Implementation;
  spimpl::impl_ptr<Implementation> impl;

  // TODO: With some if-constexpr, I could move all these into a single method
  // in a shared private base class.
  friend cereal::access;
  template <typename Archive>
  void save(Archive &) const;
  template <typename Archive>
  void load(Archive &);
};
}  // namespace flatsurf

// Makes Vertex hashable.
template <>
struct std::hash<flatsurf::Vertex> {
  size_t operator()(const flatsurf::Vertex &) const noexcept;
};

#endif
