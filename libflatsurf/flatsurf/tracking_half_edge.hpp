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

#ifndef LIBFLATSURF_TRACKING_HALF_EDGE_HPP
#define LIBFLATSURF_TRACKING_HALF_EDGE_HPP

#include <functional>

#include <boost/operators.hpp>

#include "flat_triangulation_combinatorial.hpp"
#include "vertex.hpp"

namespace flatsurf {
class TrackingHalfEdge : boost::equality_comparable<TrackingHalfEdge> {
 public:
  // The parent does not need to remain valid. If it is destructed, it will
  // signal the TrackingHalfEdge so that it removes its reference to it.
  TrackingHalfEdge(const FlatTriangulationCombinatorial *parent, HalfEdge value, bool followFlip = true);

  ~TrackingHalfEdge();

  operator HalfEdge() const;
  operator Edge() const;
  HalfEdge operator-() const;
  bool operator==(const TrackingHalfEdge &) const;
  TrackingHalfEdge &operator=(HalfEdge);

  friend std::ostream &operator<<(std::ostream &, const TrackingHalfEdge &);

  const FlatTriangulationCombinatorial &parent() const noexcept;

 private:
  class Implementation;
  spimpl::unique_impl_ptr<Implementation> impl;
};
}  // namespace flatsurf

#endif
