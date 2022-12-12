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
#include "../flatsurf/fmt.hpp"
#include "../flatsurf/ccw.hpp"
#include "../flatsurf/deformation.hpp"
#include "util/assert.ipp"
#include "util/hash.ipp"

namespace flatsurf {

namespace {

template <typename T>
T div(const T& numerator, const T& denominator) {
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
}

}

template <typename Surface>
LinearEquivalence<Surface>::LinearEquivalence(bool oriented, Normalization normalization, Predicate predicate) :
  oriented(oriented),
  normalization(normalization),
  predicate(predicate) {}

template <typename Surface>
std::vector<Deformation<Surface>> LinearEquivalence<Surface>::automorphisms(const Surface&) const {
  throw std::logic_error("not implemented: LinearEquivalence::automorphisms()");
}

template <typename Surface>
Deformation<Surface> LinearEquivalence<Surface>::normalize(const Surface&) const {
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
      HalfEdge next = start;
      while (true) {
        next = surface.nextAtVertex(next);

        LIBFLATSURF_ASSERT_ARGUMENT(next != start, "edge graph singled out by predicate must not contain cut edges such as leafs");

        if (predicate == nullptr || predicate(surface, next)) {
          const auto normalizationMatrix = normalize(surface, start, next);
          LIBFLATSURF_ASSERT(std::get<0>(normalizationMatrix) * std::get<3>(normalizationMatrix) - std::get<1>(normalizationMatrix) * std::get<2>(normalizationMatrix) > 0, "normalization must preserve orientation");
          walkers.push_back({&surface, start, &predicate, normalizationMatrix});
          break;
        }
      }
    }

    if (!oriented) {
      HalfEdge next = start;

      while (true) {
        next = surface.previousAtVertex(next);

        LIBFLATSURF_ASSERT_ARGUMENT(next != start, "edge graph singled out by predicate must not contain cut edges such as leafs");

        if (predicate == nullptr || predicate(surface, next)) {
          const auto normalizationMatrix = normalize(surface, start, surface.previousAtVertex(start));
          LIBFLATSURF_ASSERT(std::get<0>(normalizationMatrix) * std::get<3>(normalizationMatrix) - std::get<1>(normalizationMatrix) * std::get<2>(normalizationMatrix) < 0, "normalization must not preserve orientation");
          walkers.push_back({&surface, start, &predicate, normalizationMatrix});
          break;
        }
      }
    }
  }

  return LinearEquivalenceWalker<Surface>::word(std::move(walkers));
}

template <typename Surface>
typename LinearEquivalence<Surface>::Matrix LinearEquivalence<Surface>::normalize(const Surface& surface, HalfEdge e, HalfEdge f) const {
  if (std::holds_alternative<GROUP>(this->normalization)) {
    switch(std::get<GROUP>(this->normalization)) {
      case GROUP::TRIVIAL:
        return {T(1), T(), T(), T(1)};
      case GROUP::GL:
        return orthonormalize(surface, e, f);
      case GROUP::SL:
        return orthogonalize(surface, e, f);
      case GROUP::O:
        return rotate(surface, e, f);
      default:
        throw std::logic_error("not implemented: normalization with this linear subgroup not implemented");
    }
  }

  return std::get<std::function<Matrix(const Surface&, HalfEdge, HalfEdge)>>(this->normalization)(surface, e, f);
}

template <typename Surface>
typename LinearEquivalence<Surface>::Matrix LinearEquivalence<Surface>::rotate(const Surface& surface, HalfEdge e, HalfEdge f) {
  const auto v = surface.fromHalfEdge(e);
  const auto w = surface.fromHalfEdge(f);

  // We would like to determine the rotation matrix that maps (x, y) to (1, 0).
  // However, to compute this matrix, we'd have to take square roots which
  // might not exist in the base ring.

  // Instead, we use the matrix [[x, y], [-y, x]] that takes (x, y) to (x^2 + y^2, 0).
  // That matrix is a scalar multiple of the rotation matrix.

  if (v.ccw(surface.fromHalfEdge(f)) == CCW::COUNTERCLOCKWISE)
    return { v.x(), v.y(), -v.y(), v.x() };
  else
    return { v.x(), v.y(), v.y(), -v.x() };
}

template <typename Surface>
typename LinearEquivalence<Surface>::Matrix LinearEquivalence<Surface>::orthogonalize(const Surface& surface, HalfEdge e, HalfEdge f) {
  // We determine the matrix in SL2± that maps e to (1, 0) and f to some (0, y),
  // to do this we determine the matrix that maps to (1, 0) and (0, 1) and the
  // scale the second row.
  const auto [a, b, c, d] = orthonormalize(surface, e,  f);

  T det = a*d - b*c;

  if (det < 0)
    det = -det;

  return { a, b, div(c, det), div(d, det) };
}

template <typename Surface>
typename LinearEquivalence<Surface>::Matrix LinearEquivalence<Surface>::orthonormalize(const Surface& surface, HalfEdge a, HalfEdge b) {
  const auto v = surface.fromHalfEdge(a);
  const auto w = surface.fromHalfEdge(b);

  const T det = v.x() * w.y() - v.y() * w.x();

  LIBFLATSURF_ASSERT(det, "normalization was presented with collinear edges " << a << " and " << b);

  return {div<T>(w.y(), det), div<T>(-w.x(), det), div<T>(-v.y(), det), div<T>(v.x(), det)};
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const LinearEquivalence<Surface>& equivalence) {
  return os << equivalence.toString();
}

}

#include "../flatsurf/vector.hpp"
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), LinearEquivalence, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
