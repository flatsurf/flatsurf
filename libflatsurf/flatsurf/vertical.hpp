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

#ifndef LIBFLATSURF_VERTICAL_HPP
#define LIBFLATSURF_VERTICAL_HPP

#include <iosfwd>
#include <set>
#include <vector>
#include <memory>
#include <type_traits>

#include <boost/operators.hpp>

#include "external/spimpl/spimpl.h"

#include "forward.hpp"

namespace flatsurf {
// TODO: Maybe split into a base class Direction or Orientation or something
// like that and this subclass Vertical which is attached to a Surface.
template <class Surface>
class Vertical : boost::equality_comparable<Vertical<Surface>> {

  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

 public:
  Vertical(std::shared_ptr<const Surface>, const typename Surface::Vector& vertical);

  enum class TRIANGLE {
    BACKWARD = 1,
    FORWARD = -1,
    COLLAPSED_TO_TWO_FACES = 0,
    LEFT_VERTICAL = 2,
    RIGHT_VERTICAL = -2,
  };

  TRIANGLE classifyFace(HalfEdge) const;

  // Return whether this is a large edge, i.e., whether it is a wide edge of
  // both of its adjacent triangles.
  bool large(HalfEdge) const;

  const typename Surface::Vector& vertical() const;
  const typename Surface::Vector& horizontal() const;

  typename Surface::Coordinate perpendicular(const typename Surface::Vector&) const;
  typename Surface::Coordinate parallel(const typename Surface::Vector&) const;
  bool parallel(Edge) const;
  bool perpendicular(Edge) const;
  bool parallel(HalfEdge) const;
  bool perpendicular(HalfEdge) const;

  std::shared_ptr<const Surface> surface() const;

  std::vector<std::set<HalfEdge>> components() const;

  bool operator==(const Vertical &) const noexcept;

  Vertical<Surface> operator-() const;

  template <typename S>
  friend std::ostream &operator<<(std::ostream &, const Vertical<S> &);

 // TODO
 public:
  using Implementation = ::flatsurf::Implementation<Vertical>;
  spimpl::impl_ptr<Implementation> impl;

  friend Implementation;

  friend cereal::access;
  template <typename Archive>
  void save(Archive &archive) const;
  template <typename Archive>
  void load(Archive &archive);
};

template <typename Surface>
Vertical(std::shared_ptr<const Surface>, const typename Surface::Vector&) -> Vertical<Surface>;
}  // namespace flatsurf

#endif
