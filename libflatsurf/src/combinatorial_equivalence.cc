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

#include "impl/combinatorial_equivalence.hpp"

#include "../flatsurf/equivalence_class_code.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/iterable.hpp"
#include "util/assert.ipp"
#include "util/hash.ipp"

namespace flatsurf {

template <typename Surface>
std::unique_ptr<EquivalenceClassCode> CombinatorialEquivalence<Surface>::code(const Surface& surface) const {
  // Pick a random half edge as the starting point and call it half edge 1.
  // Then walk the surface in a breadth-first search, labeling half edges as 2,
  // 3, ... as we find unlabeled ones. The sequence of half edges we see during
  // this walk is a code for this surface. If we sort the codes and pick the
  // lexicographically largest one, then it uniquely describes this surface
  // combinatorially.

  // To speed things up we compute all the codes in parallel and discard codes
  // that are not the lexicographically largest immediately.
  // As an optimization we don't need to record the label of the edge we are
  // crossing since it just going to be smallest edge label we have seen so
  // far.

  // Note that things get a bit more complicated here because we support that
  // there is a predicate on the half edges that singles out possibly
  // non-triangular cells that should be the only relevant data for this notion
  // of equivalence.

  using Word = std::vector<int>;
  using Code = std::vector<Word>;

  Code code;

  struct Walker {
    // Map half edges to 0, 1, 2, … as we encounter them when walking the
    // surface. By construction this will only contain a single signed half
    // edge for each edge.
    // (Note that we don't use a HalfEdge map since it is allocating a linear
    // amount of memory from the start which would mean that we'd get quadratic
    // runtime by just initializing linearly many runners.)
    std::unordered_map<HalfEdge, int> labels;

    // The set of labeled half edges in the order they were encountered.
    // Essentially, this is the inverse mapping of labels.
    std::vector<HalfEdge> labeled;

    Word word(const Surface& surface, const HalfEdge cross, const Predicate& predicate) {
      Word word;

      const HalfEdge start = -cross;

      if (labels.find(start) != std::end(labels))
        // We have processed this face before. This creates an empty word.
        return word;

      // Build the word by walking the face of start.
      HalfEdge pos = start;

      while (true) {
        LIBFLATSURF_ASSERT(predicate(surface, pos), "Cannot record an edge that has been filtered out by the equivalence predicate.");

        pos = surface.nextInFace(pos);

        while (!predicate(surface, pos))
          pos = surface.previousAtVertex(pos);

        if (pos == start)
          break;

        word.push_back(label(surface, pos));
      }

      return word;
    }

    int label(const Surface& surface, const HalfEdge halfEdge) {
      if (halfEdge == labeled.at(0))
        return 0;

      LIBFLATSURF_ASSERT(labels.find(halfEdge) == std::end(labels), "We found a known label " << labels[halfEdge] << " for half edge " << halfEdge << " when starting our walk at half edge " << labeled.at(0) << " on " << surface);
      
      if (labels.find(-halfEdge) != std::end(labels))
        // We have seen this edge before.
        return -labels[-halfEdge];

      // We have never seen this edge before. Label it.
      const int label = static_cast<int>(labels.size());
      labels[halfEdge] = label;
      labeled.push_back(halfEdge);
      return label;
    }
  };

  std::vector<Walker> walkers;

  for (const auto start : surface.halfEdges()) {
    if (!predicate(surface, start))
      continue;

    Walker walker;
    walker.labels[-start] = 0;
    walker.labeled.push_back(-start);

    walkers.push_back(walker);
  }

  std::vector<Walker*> maximalWalkers;

  for (auto& walker: walkers)
    maximalWalkers.push_back(&walker);

  while (code.size() < std::begin(walkers)->labeled.size()) {
    std::vector<Walker*> stillMaximalWalkers;

    Word maximalWord;

    for (auto* walker : maximalWalkers) {
      // We create a new word for our code by crossing the code.size()'th half edge we have labeled.
      const HalfEdge cross = walker->labeled.at(code.size());

      const Word word = walker->word(surface, cross, predicate);

      if (word > maximalWord) {
        maximalWord = word;
        stillMaximalWalkers = {walker};
      } else if (word == maximalWord) {
        stillMaximalWalkers.push_back(walker);
      } else {
        // We drop this walker since it leads to a non-maximal code.
        ;
      }
    }

    code.push_back(maximalWord);

    maximalWalkers = std::move(stillMaximalWalkers);
  }

  return std::make_unique<CombinatorialEquivalenceClassCode>(code);
}

template <typename Surface>
Iterable<Deformation<Surface>> CombinatorialEquivalence<Surface>::automorphisms() const {
  throw std::logic_error("not implemented: CombinatorialEquivalence::automorphisms()");
}

template <typename Surface>
void CombinatorialEquivalence<Surface>::normalize(Surface&) const {
  throw std::logic_error("not implemented: CombinatorialEquivalence::normalize()");
}

template <typename Surface>
bool CombinatorialEquivalence<Surface>::equal(const ImplementationOf<Equivalence<Surface>>&) const {
  throw std::logic_error("not implemented: CombinatorialEquivalence::equal()");
}

template <typename Surface>
std::string CombinatorialEquivalence<Surface>::toString() const {
  throw std::logic_error("not implemented: CombinatorialEquivalence::toString()");
}

template <typename Surface>
CombinatorialEquivalence<Surface>::CombinatorialEquivalenceClassCode::CombinatorialEquivalenceClassCode(Code code) : code(std::move(code)) {}

template <typename Surface>
size_t CombinatorialEquivalence<Surface>::CombinatorialEquivalenceClassCode::hash() const {
  size_t hash = code.size();
  for (const auto& word: code)
    for (const auto& character: word)
      hash = hash_combine(hash, character);
  return hash;
}

template <typename Surface>
bool CombinatorialEquivalence<Surface>::CombinatorialEquivalenceClassCode::equal(const EquivalenceClassCode& rhs) const {
  const CombinatorialEquivalenceClassCode* other = dynamic_cast<const CombinatorialEquivalenceClassCode*>(&rhs);
  if (other == nullptr)
    return false;

  return code == other->code;
}

template <typename Surface>
std::string CombinatorialEquivalence<Surface>::CombinatorialEquivalenceClassCode::toString() const {
  return fmt::format("{}", code);
}

template <typename Surface>
std::ostream& operator<<(std::ostream&, const CombinatorialEquivalence<Surface>&) {
  throw std::logic_error("not implemented: CombinatorialEquivalence ostream operator<<");
}

}

#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), CombinatorialEquivalence, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
