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

#include <unordered_map>

#include <fmt/format.h>
#include <fmt/ranges.h>

#include "impl/combinatorial_equivalence_class_code.hpp"

#include "util/hash.ipp"
#include "util/assert.ipp"

namespace flatsurf {

CombinatorialEquivalenceClassCode::CombinatorialEquivalenceClassCode(Word word) : word(std::move(word)) {
  LIBFLATSURF_ASSERT(([&]() {
    const auto& word = this->word;

    int min = 0;
    int max = 0;

    std::unordered_map<int, int> labels;

    for (int c = 0; c < word.size(); c++) {
      const auto& character = word[c];

      if (character.size() == 0)
        continue;

      min = std::min(min, -c);
      labels[c] += 2;

      for (const auto& label: character) {
        if (label < 0) {
          min = std::min(min, label);
          labels[-label] += 2;
        } else {
          max = std::max(max, label);
          labels[label] += 1;
        }
      }
    }

    if (max != -min)
      return false;

    for (int label = 0; label <= max; label++)
      if (labels[label] != 3)
        return false;

    return true;
  }()), "malformed combinatorial code word describing equivalence class, each nonzero label must show up exactly once with each sign: " << *this);
}

size_t CombinatorialEquivalenceClassCode::hash() const {
  size_t hash = word.size();
  for (const auto& character: word)
    for (const auto& stroke: character)
      hash = hash_combine(hash, stroke);
  return hash;
}

bool CombinatorialEquivalenceClassCode::equal(const EquivalenceClassCode& rhs) const {
  const CombinatorialEquivalenceClassCode* other = dynamic_cast<const CombinatorialEquivalenceClassCode*>(&rhs);
  if (other == nullptr)
    return false;

  return word == other->word;
}

std::string CombinatorialEquivalenceClassCode::toString() const {
  return fmt::format("{}", word);
}

}
