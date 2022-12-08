/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2022 Julian Rüth
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

#include <stdexcept>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <exact-real/forward.hpp>

#include "impl/linear_equivalence.hpp"
#include "impl/linear_equivalence_walker.hpp"

#include "impl/equivalence_class_code.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/iterable.hpp"
#include "../flatsurf/fmt.hpp"
#include "util/assert.ipp"
#include "util/hash.ipp"

namespace flatsurf {

template <typename Surface>
LinearEquivalence<Surface>::LinearEquivalence(bool oriented, Normalization normalization, Predicate predicate) :
  oriented(oriented),
  normalization(normalization),
  predicate(predicate) {}

template <typename Surface>
Iterable<Deformation<Surface>> LinearEquivalence<Surface>::automorphisms() const {
  throw std::logic_error("not implemented: LinearEquivalence::automorphisms()");
}

template <typename Surface>
void LinearEquivalence<Surface>::normalize(Surface&) const {
  throw std::logic_error("not implemented: LinearEquivalence::normalize()");
}

template <typename Surface>
bool LinearEquivalence<Surface>::equal(const ImplementationOf<Equivalence<Surface>>& o) const {
  const auto* other = dynamic_cast<const LinearEquivalence*>(&o);

  if (other == nullptr)
    return false;

  if (this == other)
    return true;

  return oriented == other->oriented && (normalization == nullptr) == (other->normalization == nullptr) && (predicate == nullptr) == (other->predicate == nullptr);
}

template <typename Surface>
std::string LinearEquivalence<Surface>::toString() const {
  throw std::logic_error("not implemented: LinearEquivalence::toString()");
}

template <typename Surface>
std::unique_ptr<EquivalenceClassCode> LinearEquivalence<Surface>::code(const Surface& surface) const {
  std::vector<LinearEquivalenceWalker<Surface>> walkers;

  const auto orthonormalization = [](const Surface& surface, HalfEdge a, HalfEdge b) -> Matrix {
    const auto v = surface.fromHalfEdge(a);
    const auto w = surface.fromHalfEdge(b);

    const T det = v.x() * w.y() - v.y() * w.x();

    LIBFLATSURF_ASSERT(det, "orthonormalization was presented with collinear edges " << a << " and " << b);

    const auto div = [](const auto& numerator, const auto& denominator) {
      if constexpr (std::is_same_v<T, exactreal::Element<exactreal::IntegerRing>> || std::is_same_v<T, exactreal::Element<exactreal::RationalField>> || std::is_same_v<T, exactreal::Element<exactreal::NumberField>>) {
        const auto maybe = numerator.truediv(denominator);
        if (!maybe)
          throw std::logic_error("orthonormalization is not possible in this ring");

        return *maybe;
      } else {
        const T quot = numerator / denominator;
        if (quot * denominator != numerator)
          throw std::logic_error("orthonormalization is not possible in this ring");

        return quot;
      }
    };

    return {div(w.y(), det), div(-w.x(), det), div(-v.y(), det), div(v.x(), det)};
  };

  Normalization normalization = this->normalization == nullptr ? orthonormalization : this->normalization;

  for (const auto start : surface.halfEdges()) {
    if (predicate != nullptr && !predicate(surface, start))
      continue;

    {
      const auto normalizationMatrix = normalization(surface, start, surface.nextAtVertex(start));
      LIBFLATSURF_ASSERT(std::get<0>(normalizationMatrix) * std::get<3>(normalizationMatrix) - std::get<1>(normalizationMatrix) * std::get<2>(normalizationMatrix) > 0, "normalization must preserve orientation");
      walkers.push_back({&surface, start, &predicate, normalizationMatrix});
    }

    if (!oriented) {
      const auto normalizationMatrix = normalization(surface, start, surface.previousAtVertex(start));
      LIBFLATSURF_ASSERT(std::get<0>(normalizationMatrix) * std::get<3>(normalizationMatrix) - std::get<1>(normalizationMatrix) * std::get<2>(normalizationMatrix) < 0, "normalization must not preserve orientation");
      walkers.push_back({&surface, start, &predicate, normalizationMatrix});
    }
  }

  return LinearEquivalenceWalker<Surface>::word(std::move(walkers));
}

template <typename Surface>
std::ostream& operator<<(std::ostream&, const LinearEquivalence<Surface>&) {
  throw std::logic_error("not implemented: LinearEquivalence ostream operator<<");
}

}

#include "../flatsurf/vector.hpp"
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), LinearEquivalence, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
