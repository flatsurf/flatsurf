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
#include <optional>
#include <vector>
#include "external/spimpl/spimpl.h"

#include "flatsurf/forward.hpp"
#include "flatsurf/half_edge.hpp"

namespace flatsurf {
template <typename Surface>
class SaddleConnection : boost::equality_comparable<SaddleConnection<Surface>> {
  using Vector = typename Surface::Vector;

 public:
  const Vector &vector() const;
  // The saddle connection is leaving from the vertex at the source of source.
  // It is leaving in a direction that is contained in the sector next to
  // source (counterclockwise.)
  HalfEdge source() const;
  // The saddle connection is reaching the vertex at the target of target.
  // It is approching in a direction that is contained in the sector
  // *clockwise* from *-target*.
  HalfEdge target() const;

  const Surface &surface() const;

  std::vector<HalfEdge> crossings() const;

  std::optional<int> angle(const SaddleConnection<Surface> &) const;

  SaddleConnection<Surface> operator-() const noexcept;

  bool operator==(const SaddleConnection<Surface> &) const;

  // See cppyy.hpp for the _ parameter.
  template <typename Surf, typename _>
  friend std::ostream &operator<<(std::ostream &, const SaddleConnection<Surf> &);

 private:
  SaddleConnection(const std::shared_ptr<const Surface> &, HalfEdge source, HalfEdge target, const Vector &);

  friend SaddleConnections<Surface>;

  friend cereal::access;
  template <typename Archive>
  void save(Archive &archive) const;
  template <typename Archive>
  void load(Archive &archive);

  class Implementation;
  spimpl::impl_ptr<Implementation> impl;
};
}  // namespace flatsurf

#endif
