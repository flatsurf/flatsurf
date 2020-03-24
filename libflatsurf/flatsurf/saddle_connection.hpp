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

#ifndef LIBFLATSURF_SADDLE_CONNECTION_HPP
#define LIBFLATSURF_SADDLE_CONNECTION_HPP

#include <boost/operators.hpp>
#include <iosfwd>
#include <memory>
#include <optional>
#include <vector>

#include <gmpxx.h>

#include "bound.hpp"
#include "copyable.hpp"
#include "half_edge.hpp"
#include "serializable.hpp"

namespace flatsurf {
template <typename Surface>
class SaddleConnection : public Serializable<SaddleConnection<Surface>>,
                         boost::equality_comparable<SaddleConnection<Surface>>,
                         boost::less_than_comparable<SaddleConnection<Surface>, Bound> {
  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

  using T = typename Surface::Coordinate;

 public:
  SaddleConnection(std::shared_ptr<const Surface>, HalfEdge e);
  SaddleConnection(std::shared_ptr<const Surface>, HalfEdge source, HalfEdge target, const Chain<Surface> &);

  static SaddleConnection<Surface> inSector(std::shared_ptr<const Surface>, HalfEdge source, const Vector<T> &);
  static SaddleConnection<Surface> inSector(std::shared_ptr<const Surface>, HalfEdge source, const Vertical<Surface> &direction);
  static SaddleConnection<Surface> inHalfPlane(std::shared_ptr<const Surface>, HalfEdge side, const Vertical<Surface> &, const Vector<T> &);
  static SaddleConnection<Surface> inPlane(std::shared_ptr<const Surface>, HalfEdge plane, const Vector<T> &);
  static SaddleConnection<Surface> alongVertical(std::shared_ptr<const Surface>, const Vertical<Surface> &direction, HalfEdge plane);
  static SaddleConnection<Surface> clockwise(const SaddleConnection &from, const Vector<T> &);
  static SaddleConnection<Surface> reconstruct(std::shared_ptr<const Surface>, HalfEdge source, std::function<bool(const SaddleConnectionsIterator<Surface> &)> until, std::function<CCW(const SaddleConnectionsIterator<Surface> &)> skip, Bound = Bound(INT_MAX, 0));

  const Vector<T> &vector() const;
  const Chain<Surface> &chain() const;

  operator const Vector<T> &() const;
  operator const Chain<Surface> &() const;

  // The saddle connection is leaving from the vertex at the source of source.
  // It is leaving in a direction that is contained in the sector next to
  // source (counterclockwise) inclusive.
  HalfEdge source() const;
  // Symmetrical to source(), the connection is such that -vector() is in the
  // sector counterclockwise next to target() inclusive.
  HalfEdge target() const;

  const Surface &surface() const;

  std::vector<HalfEdge> crossings() const;

  std::optional<int> angle(const SaddleConnection<Surface> &) const;

  SaddleConnection<Surface> operator-() const noexcept;

  bool operator==(const SaddleConnection<Surface> &) const;

  bool operator>(const Bound) const;
  bool operator<(const Bound) const;

  // Strangely, when we do not put the _ here and try to print a
  // FlatTriangulation<eantic::renf_elem_class> through cppyy, it would compile
  // code that looks sane but fail because the overload resolution picked this
  // overload (which seems to be completely unrelated.) This fails because
  // renf_elem_class, does not have a ::Vector which SaddleConnection class
  // requires. This is clearly a bug in cppyy, but we have not been able to
  // create a minimal reproducer yet.
  template <typename Surf, typename _>
  friend std::ostream &operator<<(std::ostream &, const SaddleConnection<Surf> &);

 private:
  using Implementation = ImplementationOf<SaddleConnection>;
  Copyable<Implementation> impl;

  friend Implementation;
};

// See above for this weird construction.
template <typename Surface, typename _ = Vector<typename Surface::Coordinate>>
std::ostream &operator<<(std::ostream &, const SaddleConnection<Surface> &);

template <typename Surface, typename... T>
SaddleConnection(std::shared_ptr<const Surface>, T &&...)->SaddleConnection<Surface>;

template <typename Surface, typename... T>
SaddleConnection(std::shared_ptr<Surface>, T &&...)->SaddleConnection<Surface>;

}  // namespace flatsurf

namespace std {

template <typename Surface>
struct hash<::flatsurf::SaddleConnection<Surface>> { size_t operator()(const ::flatsurf::SaddleConnection<Surface> &) const noexcept; };

}  // namespace std

#endif
