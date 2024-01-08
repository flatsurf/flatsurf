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

#include <fmt/format.h>
#include <fmt/ranges.h>

#include "../flatsurf/fmt.hpp"

#include "impl/linear_equivalence_class_code.hpp"

#include "util/hash.ipp"

namespace flatsurf {

template <typename Surface>
LinearEquivalenceClassCode<Surface>::LinearEquivalenceClassCode(Word word) : word(std::move(word)) {}

template <typename Surface>
size_t LinearEquivalenceClassCode<Surface>::hash() const {
  const auto& combinatorial = std::get<0>(word);

  size_t hash = combinatorial.size();
  for (const auto& word: combinatorial)
    for (const auto& character: word)
      hash = hash_combine(hash, character);

  const auto& geometric = std::get<1>(word);
  for (const auto& vector : geometric)
    hash = hash_combine(hash, std::hash<Vector<T>>{}(vector));

  return hash;
}

template <typename Surface>
bool LinearEquivalenceClassCode<Surface>::equal(const EquivalenceClassCode& rhs) const {
  const LinearEquivalenceClassCode* other = dynamic_cast<const LinearEquivalenceClassCode*>(&rhs);
  if (other == nullptr)
    return false;

  return word == other->word;
}

template <typename Surface>
std::string LinearEquivalenceClassCode<Surface>::toString() const {
  return fmt::format("({}, {})", std::get<0>(word), std::get<1>(word));
}

}

#include "../flatsurf/vector.hpp"
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_STATIC), LinearEquivalenceClassCode, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
