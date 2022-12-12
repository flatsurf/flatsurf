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

#include "../flatsurf/deformation.hpp"
#include "../flatsurf/edge.hpp"
#include "impl/equivalence_class_code.hpp"
#include "impl/combinatorial_equivalence_walker.hpp"
#include "util/assert.ipp"
#include "util/hash.ipp"

namespace flatsurf {

template <typename Surface>
CombinatorialEquivalence<Surface>::CombinatorialEquivalence(bool oriented, Predicate predicate):
  oriented(oriented),
  predicate(predicate) {}

template <typename Surface>
std::vector<Deformation<Surface>> CombinatorialEquivalence<Surface>::automorphisms(const Surface&) const {
  throw std::logic_error("not implemented: CombinatorialEquivalence::automorphisms()");
}

template <typename Surface>
Deformation<Surface> CombinatorialEquivalence<Surface>::normalize(const Surface&) const {
  throw std::logic_error("not implemented: CombinatorialEquivalence::normalize()");
}

template <typename Surface>
bool CombinatorialEquivalence<Surface>::equal(const ImplementationOf<Equivalence<Surface>>& o) const {
  const auto* other = dynamic_cast<const CombinatorialEquivalence*>(&o);

  if (other == nullptr)
    return false;

  if (this == other)
    return true;

  return oriented == other->oriented && (predicate == nullptr) == (other->predicate == nullptr);
}

template <typename Surface>
std::string CombinatorialEquivalence<Surface>::toString() const {
  if (predicate != nullptr)
    return oriented ? "Orientation Preserving Combinatorial Equivalence" : "Combinatorial Equivalence";

  return oriented ? "Orientation Preserving Combinatorial Equivalence" : "Combinatorial Equivalence";
}

template <typename Surface>
std::unique_ptr<EquivalenceClassCode> CombinatorialEquivalence<Surface>::code(const Surface& surface) const {
  std::vector<CombinatorialEquivalenceWalker<Surface>> walkers;

  for (const auto start : surface.halfEdges()) {
    if (predicate != nullptr && !predicate(surface, start))
      continue;

    walkers.push_back({&surface, start, 1, &predicate});

    if (!oriented)
      walkers.push_back({&surface, start, -1, &predicate});
  }

  return CombinatorialEquivalenceWalker<Surface>::word(std::move(walkers));
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const CombinatorialEquivalence<Surface>& self) {
  return os << self.toString();
}

}

#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), CombinatorialEquivalence, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
