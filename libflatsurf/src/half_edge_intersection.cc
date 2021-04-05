/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2021 Julian Rüth
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

#include <exact-real/yap/arb.hpp>

#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/half_edge_intersection.hpp"
#include "../flatsurf/vector.hpp"

#include "impl/half_edge_intersection.impl.hpp"
#include "impl/approximation.hpp"
#include "util/assert.ipp"

#include <ostream>

namespace flatsurf {

template <typename Surface>
HalfEdge HalfEdgeIntersection<Surface>::halfEdge() const {
  return self->halfEdge;
}

template <typename Surface>
bool HalfEdgeIntersection<Surface>::operator<(const HalfEdgeIntersection& rhs) const {
  const auto relative = self->relative();
  const auto relative_ = rhs.self->relative();

  return std::get<0>(relative) * std::get<1>(relative_) < std::get<0>(relative_) * std::get<1>(relative);
}

template <typename Surface>
double HalfEdgeIntersection<Surface>::at() const {
  const auto relative = self->relative();

  for (long prec = 64;; prec *= 2) {
    const exactreal::Arb quotient = (Approximation<T>::arb(std::get<0>(relative), prec) / Approximation<T>::arb(std::get<1>(relative), prec))(prec);

    auto bounds = static_cast<std::pair<exactreal::Arf, exactreal::Arf>>(quotient);
    auto lower = static_cast<double>(bounds.first);
    auto upper = static_cast<double>(bounds.second);

    if (upper == lower)
      return upper;
  }
}

template <typename Surface>
std::tuple<typename Surface::Coordinate, typename Surface::Coordinate, typename Surface::Coordinate> ImplementationOf<HalfEdgeIntersection<Surface>>::projective() const {
  // The two lines in projective space.
  const std::tuple<T, T, T> line{-direction.y(), direction.x(), direction.y() * base.x() - direction.x() * base.y()};
  const std::tuple<T, T, T> line_{-surface->fromHalfEdge(halfEdge).y(), surface->fromHalfEdge(halfEdge).x(), T{}};

  return std::tuple{
    std::get<2>(line_) * std::get<1>(line) - std::get<2>(line) * std::get<1>(line_),
    std::get<2>(line) * std::get<0>(line_) - std::get<2>(line_) * std::get<0>(line),
    std::get<1>(line_) * std::get<0>(line) - std::get<0>(line_) * std::get<1>(line),
  };
}

template <typename Surface>
std::tuple<typename Surface::Coordinate, typename Surface::Coordinate> ImplementationOf<HalfEdgeIntersection<Surface>>::relative() const {
  const auto projective = this->projective();
  LIBFLATSURF_ASSERT(std::get<2>(projective), "HalfEdge and SaddleConnection must intersect at a finite point.");

  LIBFLATSURF_ASSERT(std::get<0>(projective) || std::get<1>(projective), "HalfEdge and SaddleConnection must not intersect at the end point of the half edge but " << base << " + λ·" << direction << " intersects " << halfEdge << " in its origin.");

  LIBFLATSURF_ASSERT((std::get<0>(projective) == 0) == (surface->fromHalfEdge(halfEdge).x() == 0), "Intersection point must be on the half edge.");
  LIBFLATSURF_ASSERT((std::get<1>(projective) == 0) == (surface->fromHalfEdge(halfEdge).y() == 0), "Intersection point must be on the half edge.");

  if (std::get<0>(projective))
    return {std::get<0>(projective), std::get<2>(projective) * surface->fromHalfEdge(halfEdge).x()};
  else
    return {std::get<1>(projective), std::get<2>(projective) * surface->fromHalfEdge(halfEdge).y()};
}

template <typename Surface>
ImplementationOf<HalfEdgeIntersection<Surface>>::ImplementationOf(const Surface& surface, const Vector<T>& base, HalfEdge halfEdge, const Vector<T>& direction) : surface(surface), base(base), direction(direction), halfEdge(halfEdge) {}

template <typename Surface>
HalfEdgeIntersection<Surface> ImplementationOf<HalfEdgeIntersection<Surface>>::make(const Surface& surface, const Vector<T>& base, HalfEdge cross, const Vector<T>& direction) {
  return HalfEdgeIntersection<Surface>(PrivateConstructor{}, surface, base, cross, direction);
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const HalfEdgeIntersection<Surface>& self) {
  return os << self.at() << " * " << self.halfEdge();
}

}

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), HalfEdgeIntersection, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)

