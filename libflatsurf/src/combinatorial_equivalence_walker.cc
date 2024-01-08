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

#include <unordered_set>

#include <fmt/format.h>
#include <fmt/ranges.h>

#include "../flatsurf/fmt.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/odd_half_edge_map.hpp"

#include "impl/combinatorial_equivalence_walker.hpp"
#include "impl/combinatorial_deformation_relation.hpp"
#include "impl/deformation.impl.hpp"

namespace flatsurf {

template <typename Surface>
CombinatorialEquivalenceWalker<Surface>::CombinatorialEquivalenceWalker(const Surface& surface, HalfEdge start, int orientation):
  EquivalenceWalker<Surface, CombinatorialEquivalenceWalker>(surface),
  orientation(orientation) {
  LIBFLATSURF_ASSERT(orientation == -1 || orientation == 1, "orientation must be -1 or 1 but was " << orientation);
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
    return Character{};

  // Build the word by walking the face of start.
  HalfEdge pos = start;

  while (true) {
    // When the orientation is reversed, the vertex permutation is replaced with its inverse,
    // so previousAtVertex is replaced with nextAtVertex. The replacement for
    // nextInFace is more complicated, using the fact that nextInFace(he) =
    // -nextAtVertex(-nextAtVertex() in a triangulated surface.
    pos = orientation > 0 ? this->surface->nextInFace(pos) : (-this->surface->previousAtVertex(-this->surface->previousAtVertex(pos)));

    if (pos == start)
      break;

    const int label = this->label(*this->surface, pos);

    if (label > 0 && label < this->steps)
      // We processed this face before.
      return Character{};

    if (label < 0 && -label < this->steps - 1)
      // We processed this face before.
      return Character{};

    character.push_back(label);
  }

  return character;
}

template <typename Surface>
int CombinatorialEquivalenceWalker<Surface>::cmp(const Character& lhs, const Character& rhs) {
  if (lhs < rhs)
    return -1;

  if (lhs > rhs)
    return 1;

  return 0;
}

template <typename Surface>
void CombinatorialEquivalenceWalker<Surface>::append(Word& word, const Character& character) {
  word.push_back(character);
}

template <typename Surface>
int CombinatorialEquivalenceWalker<Surface>::label(const Surface&, const HalfEdge halfEdge) {
  if (halfEdge == labeled.at(0))
    return 0;

  {
    const auto label = labels.find(halfEdge);

    if (label != std::end(labels))
      // We have seen this edge before.
      return label->second;
  }

  {
    const auto label = labels.find(-halfEdge);

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

template <typename Surface>
Surface CombinatorialEquivalenceWalker<Surface>::normalization() const {
  return this->surface->relabel(Permutation<HalfEdge>(relabeling())).codomain().clone();
}

template <typename Surface>
Deformation<Surface> CombinatorialEquivalenceWalker<Surface>::deformation(const Surface& normalization) const {
  return ImplementationOf<Deformation<Surface>>::make(std::make_unique<CombinatorialDeformationRelation<Surface>>(*this->surface, normalization, relabeling()));
}

template <typename Surface>
std::unordered_map<HalfEdge, HalfEdge> CombinatorialEquivalenceWalker<Surface>::relabeling() const {
  std::unordered_map<HalfEdge, HalfEdge> permutation;

  std::vector<bool> seen(2*this->surface->size());

  for (const auto& preimage: this->surface->halfEdges()) {
    auto image = labels.find(preimage);

    if (image != std::end(labels)) {
      // Note that we need to shift the value since image->second is zero-based.
      permutation[preimage] = image->second + 1;
      continue;
    }

    image = labels.find(-preimage);

    LIBFLATSURF_ASSERT(image != std::end(labels), "half edge not in any cell");

    // Note that we need to shift the value since image->second is zero-based.
    permutation[preimage] = -(image->second + 1);
  }

  LIBFLATSURF_ASSERT(permutation.size() == 2*this->surface->size(), "relabeling not defined on all half edges of surface");

  return permutation;
}

}


#include "../flatsurf/vector.hpp"
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_STATIC), CombinatorialEquivalenceWalker, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
