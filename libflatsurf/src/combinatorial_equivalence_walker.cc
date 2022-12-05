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

#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/edge.hpp"

#include "impl/combinatorial_equivalence_walker.hpp"

namespace flatsurf {

template <typename Surface>
CombinatorialEquivalenceWalker<Surface>::CombinatorialEquivalenceWalker(const Surface* surface, HalfEdge start, const Predicate* predicate):
  EquivalenceWalker<Surface, CombinatorialEquivalenceWalker>(surface),
  predicate(predicate) {
  labels[-start] = 0;
  labeled.push_back(-start);
}

template <typename Surface>
std::optional<typename CombinatorialEquivalenceWalker<Surface>::Character> CombinatorialEquivalenceWalker<Surface>::step() {
  if (this->steps >= labeled.size())
    return std::nullopt;

  Character character;

  const HalfEdge cross = labeled[this->steps++];

  const HalfEdge start = -cross;

  if (labels.find(start) != std::end(labels))
    // We have processed this face before. This creates an empty word.
    return character;

  // Build the word by walking the face of start.
  HalfEdge pos = start;

  while (true) {
    LIBFLATSURF_ASSERT((*predicate)(*this->surface, pos), "Cannot record an edge that has been filtered out by the equivalence predicate.");

    pos = this->surface->nextInFace(pos);

    while (!(*predicate)(*this->surface, pos))
      pos = this->surface->previousAtVertex(pos);

    if (pos == start)
      break;

    const int label = this->label(*this->surface, pos);

    if (label > 0 && label < labels.size() - 2)
      // We found a positive label that was not freshly assigned. We have
      // processed this face before.
      return {};

    character.push_back(label);
  }

  return character;
}

template <typename Surface>
int CombinatorialEquivalenceWalker<Surface>::cmp(const std::optional<Character>& lhs, const std::optional<Character>& rhs) {
  if (!lhs.has_value()) {
    if (!rhs.has_value())
      return 0;
    return 1;
  }

  if (!rhs.has_value())
    return -1;

  if (*lhs < *rhs)
    return -1;
  
  if (*lhs > *rhs)
    return 1;
  
  return 0;
}

template <typename Surface>
void CombinatorialEquivalenceWalker<Surface>::append(Word& word, const Character& character) {
  word.push_back(character);
}

template <typename Surface>
int CombinatorialEquivalenceWalker<Surface>::label(const Surface& surface, const HalfEdge halfEdge) {
  if (halfEdge == labeled.at(0))
    return 0;

  {
    const auto& label = labels.find(halfEdge);

    if (label != std::end(labels))
      // We have seen this edge before.
      return label->second;
  }

  {
    const auto& label = labels.find(-halfEdge);

    if (label != std::end(labels))
        // We have seen this edge before.
        return -label->second;
  }

  // We have never seen this edge before. Label it.
  const int label = static_cast<int>(labels.size());
  labels[halfEdge] = label;
  labeled.push_back(halfEdge);

  return label;
}

}


#include "../flatsurf/vector.hpp"
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_STATIC), CombinatorialEquivalenceWalker, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
