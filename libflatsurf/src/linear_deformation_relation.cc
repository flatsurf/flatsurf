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

#include "impl/linear_deformation_relation.hpp"

#include <boost/type_traits/is_detected.hpp>
#include <ostream>

#include "../flatsurf/chain.hpp"
#include "../flatsurf/deformation.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/path_iterator.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/vertex.hpp"
#include "impl/deformation.impl.hpp"

namespace flatsurf {

namespace {

template <typename T>
using truediv_t = decltype(std::declval<T&>() /= std::declval<const T&>());

}

template <typename Surface>
LinearDeformationRelation<Surface>::LinearDeformationRelation(const Surface& domain, const Surface& codomain, const T& a, const T& b, const T& c, const T& d) :
  DeformationRelation<Surface>(domain, codomain), a(a), b(b), c(c), d(d) {}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> LinearDeformationRelation<Surface>::clone() const {
  return std::unique_ptr<DeformationRelation<Surface>>(new LinearDeformationRelation(this->domain, this->codomain, a, b, c, d));
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> LinearDeformationRelation<Surface>::section() const {
  T aa = a;
  T bb = b;
  T cc = c;
  T dd = d;

  const T det = a * d - b * c;

  if (!truediv(aa, det) || !truediv(bb, det) || !truediv(cc, det) || !truediv(dd, det)) throw std::logic_error("Linear transformation is not invertible over the base ring.");

  return std::unique_ptr<DeformationRelation<Surface>>(new LinearDeformationRelation(this->codomain, this->domain, aa, bb, cc, dd));
}

template <typename Surface>
std::optional<Path<Surface>> LinearDeformationRelation<Surface>::operator()(const Path<Surface>& path) const {
  Path<Surface> path_;

  int positive = (a * d - b * c) > 0;

  for (const auto& segment : path) {
    Chain<Surface> chain{*this->codomain};
    for (auto e : this->domain->edges())
      chain += segment.chain()[e] * Chain<Surface>{*this->codomain, e.positive()};

    auto source = segment.source();
    auto target = segment.target();
    if (!positive) {
      source = this->domain->nextAtVertex(source);
      target = this->domain->nextAtVertex(target);
    }

    path_.push_back(SaddleConnection<Surface>::counterclockwise(*this->codomain, source, target, chain));
  }

  return path_;
}

template <typename Surface>
Point<Surface> LinearDeformationRelation<Surface>::operator()(const Point<Surface>& point) const {
  throw std::logic_error("not implemented: cannot map points with this kind of deformation yet");
}

template <typename Surface>
bool LinearDeformationRelation<Surface>::trivial() const {
  return a == 1 && b == 0 && c == 0 && d == 1;
}

template <typename Surface>
std::ostream& LinearDeformationRelation<Surface>::operator>>(std::ostream& os) const {
  return os << this->domain << " → " << this->codomain << " given by (" << this->a << ", " << this->b << ", " << this->c << ", " << this->d << ")";
}

template <typename Surface>
bool LinearDeformationRelation<Surface>::truediv(T& lhs, const T& rhs) {
  if constexpr (boost::is_detected_v<truediv_t, T>) {
    T tmp = lhs;
    lhs /= rhs;
    return lhs * rhs == tmp;
  } else {
    auto maybe = lhs.truediv(rhs);
    if (!maybe) return false;
    lhs = *maybe;
    return true;
  }
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_STATIC), LinearDeformationRelation, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
