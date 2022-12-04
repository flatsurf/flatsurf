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

#include "impl/linear_equivalence.hpp"

#include "impl/equivalence_class_code.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/iterable.hpp"
#include "../flatsurf/fmt.hpp"
#include "util/assert.ipp"
#include "util/hash.ipp"

namespace flatsurf {

template <typename Surface>
Iterable<Deformation<Surface>> LinearEquivalence<Surface>::automorphisms() const {
  throw std::logic_error("not implemented: LinearEquivalence::automorphisms()");
}

template <typename Surface>
void LinearEquivalence<Surface>::normalize(Surface&) const {
  throw std::logic_error("not implemented: LinearEquivalence::normalize()");
}

template <typename Surface>
bool LinearEquivalence<Surface>::equal(const ImplementationOf<Equivalence<Surface>>&) const {
  throw std::logic_error("not implemented: LinearEquivalence::equal()");
}

template <typename Surface>
std::string LinearEquivalence<Surface>::toString() const {
  throw std::logic_error("not implemented: LinearEquivalence::toString()");
}

template <typename Surface>
LinearEquivalence<Surface>::LinearEquivalenceClassCode::LinearEquivalenceClassCode(Code code) : code(std::move(code)) {}

template <typename Surface>
std::unique_ptr<EquivalenceClassCode> LinearEquivalence<Surface>::code(const Surface& surface) const {
  return ImplementationOf<Equivalence<Surface>>::code(seedWalkers(surface));
}

template <typename Surface>
std::vector<typename LinearEquivalence<Surface>::LinearWalker> LinearEquivalence<Surface>::seedWalkers(const Surface& surface) const {
  throw std::logic_error("not implemented: seedWalkers()");
}

template <typename Surface>
size_t LinearEquivalence<Surface>::LinearEquivalenceClassCode::hash() const {
  const auto& combinatorial = std::get<0>(code);

  size_t hash = combinatorial.size();
  for (const auto& word: combinatorial)
    for (const auto& character: word)
      hash = hash_combine(hash, character);

  const auto& geometric = std::get<1>(code);
  for (const auto& vector : geometric)
    hash = hash_combine(hash, std::hash<Vector<T>>{}(vector));

  return hash;
}

template <typename Surface>
bool LinearEquivalence<Surface>::LinearEquivalenceClassCode::equal(const EquivalenceClassCode& rhs) const {
  const LinearEquivalenceClassCode* other = dynamic_cast<const LinearEquivalenceClassCode*>(&rhs);
  if (other == nullptr)
    return false;

  return code == other->code;
}

template <typename Surface>
std::string LinearEquivalence<Surface>::LinearEquivalenceClassCode::toString() const {
  return fmt::format("({}, {})", std::get<0>(code), std::get<1>(code));
}

template <typename Surface>
std::ostream& operator<<(std::ostream&, const LinearEquivalence<Surface>&) {
  throw std::logic_error("not implemented: LinearEquivalence ostream operator<<");
}

}

#include "../flatsurf/vector.hpp"
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), LinearEquivalence, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
