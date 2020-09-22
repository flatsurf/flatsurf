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

#ifndef LIBFLATSURF_VERTICAL_HPP
#define LIBFLATSURF_VERTICAL_HPP

#include <boost/operators.hpp>
#include <iosfwd>
#include <memory>
#include <type_traits>
#include <unordered_set>
#include <vector>

#include "copyable.hpp"
#include "serializable.hpp"

namespace flatsurf {
template <typename Surface>
class Vertical : Serializable<Vertical<Surface>>,
                 boost::equality_comparable<Vertical<Surface>> {
  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

  using T = typename Surface::Coordinate;

 public:
  Vertical(const Surface&, const Vector<T> &vertical);

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

  const Vector<T> &vertical() const;
  const Vector<T> &horizontal() const;

  T perpendicular(const Vector<T> &) const;
  T parallel(const Vector<T> &) const;
  bool parallel(Edge) const;
  bool perpendicular(Edge) const;
  bool parallel(HalfEdge) const;
  bool perpendicular(HalfEdge) const;

  const Surface& surface() const;

  std::vector<std::unordered_set<HalfEdge>> components() const;

  bool operator==(const Vertical &) const noexcept;

  Vertical<Surface> operator-() const;

  template <typename S>
  friend std::ostream &operator<<(std::ostream &, const Vertical<S> &);

 private:
  Copyable<Vertical> self;

  friend ImplementationOf<Vertical>;
};

template <typename Surface, typename... Args>
Vertical(std::shared_ptr<const Surface>, Args &&...) -> Vertical<Surface>;

template <typename Surface, typename... Args>
Vertical(std::shared_ptr<Surface>, Args &&...) -> Vertical<Surface>;

template <typename Surface>
Vertical(std::shared_ptr<const Surface>, const Vector<typename Surface::Coordinate> &) -> Vertical<Surface>;
}  // namespace flatsurf

#endif
