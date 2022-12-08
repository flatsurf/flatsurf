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

  if (oriented != other->oriented)
    return false;

  if (predicate != nullptr)
    return false;

  if (other->predicate != nullptr)
    return false;

  if (!std::holds_alternative<GROUP>(normalization))
    return false;

  if (!std::holds_alternative<GROUP>(other->normalization))
    return false;

  if (std::get<GROUP>(normalization) != std::get<GROUP>(other->normalization))
    return false;

  return true;
}

template <typename Surface>
std::string LinearEquivalence<Surface>::toString() const {
  if (std::holds_alternative<GROUP>(normalization)) {
    switch(std::get<GROUP>(normalization)) {
      case GROUP::TRIVIAL:
        return "Equivalence Modulo Labels";
      case GROUP::O:
        return oriented ? "Equivalence Modulo Rotation" : "Equivalence Modulo O(2)";
      case GROUP::SL:
        return oriented ? "Equivalence Modulo SL(2)" : "Equivalence Module SL±(2)";
      case GROUP::GL:
        return oriented ? "Special Linear Equivalence" : "Linear Equivalence";
      default:
        throw std::logic_error("not implemented: unsupported subgroup of GL(2)");
    }
  }

  return "Custom Linear Equivalence";
}

template <typename Surface>
std::unique_ptr<EquivalenceClassCode> LinearEquivalence<Surface>::code(const Surface& surface) const {
  std::vector<LinearEquivalenceWalker<Surface>> walkers;

  for (const auto start : surface.halfEdges()) {
    if (predicate != nullptr && !predicate(surface, start))
      continue;

    {
      const auto normalizationMatrix = normalize(surface, start, surface.nextAtVertex(start));
      LIBFLATSURF_ASSERT(std::get<0>(normalizationMatrix) * std::get<3>(normalizationMatrix) - std::get<1>(normalizationMatrix) * std::get<2>(normalizationMatrix) > 0, "normalization must preserve orientation");
      walkers.push_back({&surface, start, &predicate, normalizationMatrix});
    }

    if (!oriented) {
      const auto normalizationMatrix = normalize(surface, start, surface.previousAtVertex(start));
      LIBFLATSURF_ASSERT(std::get<0>(normalizationMatrix) * std::get<3>(normalizationMatrix) - std::get<1>(normalizationMatrix) * std::get<2>(normalizationMatrix) < 0, "normalization must not preserve orientation");
      walkers.push_back({&surface, start, &predicate, normalizationMatrix});
    }
  }

  return LinearEquivalenceWalker<Surface>::word(std::move(walkers));
}

template <typename Surface>
typename LinearEquivalence<Surface>::Matrix LinearEquivalence<Surface>::normalize(const Surface& surface, HalfEdge a, HalfEdge b) const {
  if (std::holds_alternative<GROUP>(this->normalization)) {
    switch(std::get<GROUP>(this->normalization)) {
      case GROUP::TRIVIAL:
        return {T(1), T(), T(), T(1)};
      case GROUP::GL:
        return orthonormalize(surface, a, b);
      default:
        throw std::logic_error("not implemented: normalization with this linear subgroup not implemented");
    }
  }

  return std::get<std::function<Matrix(const Surface&, HalfEdge, HalfEdge)>>(this->normalization)(surface, a, b);
}

template <typename Surface>
typename LinearEquivalence<Surface>::Matrix LinearEquivalence<Surface>::rotate(const Surface& surface, HalfEdge a, HalfEdge b) {
  throw std::logic_error("not implemented: rotate()");
}

template <typename Surface>
typename LinearEquivalence<Surface>::Matrix LinearEquivalence<Surface>::orthogonalize(const Surface& surface, HalfEdge a, HalfEdge b) {
  throw std::logic_error("not implemented: orthogonalize()");
}

template <typename Surface>
typename LinearEquivalence<Surface>::Matrix LinearEquivalence<Surface>::orthonormalize(const Surface& surface, HalfEdge a, HalfEdge b) {
  const auto v = surface.fromHalfEdge(a);
  const auto w = surface.fromHalfEdge(b);

  const T det = v.x() * w.y() - v.y() * w.x();

  LIBFLATSURF_ASSERT(det, "normalization was presented with collinear edges " << a << " and " << b);

  const auto div = [](const auto& numerator, const auto& denominator) {
    if constexpr (std::is_same_v<T, exactreal::Element<exactreal::IntegerRing>> || std::is_same_v<T, exactreal::Element<exactreal::RationalField>> || std::is_same_v<T, exactreal::Element<exactreal::NumberField>>) {
      const auto maybe = numerator.truediv(denominator);
      if (!maybe)
        throw std::logic_error("normalization is not possible in this ring");

      return *maybe;
    } else {
      const T quot = numerator / denominator;
      if (quot * denominator != numerator)
        throw std::logic_error("normalization is not possible in this ring");

      return quot;
    }
  };

  return {div(w.y(), det), div(-w.x(), det), div(-v.y(), det), div(v.x(), det)};
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const LinearEquivalence<Surface>& equivalence) {
  return os << equivalence.toString();
}

}

#include "../flatsurf/vector.hpp"
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), LinearEquivalence, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
