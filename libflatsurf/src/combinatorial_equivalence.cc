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
#include "../flatsurf/edge.hpp"
#include "../flatsurf/iterable.hpp"
#include "util/assert.ipp"
#include "util/hash.ipp"

namespace flatsurf {

template <typename Surface>
CombinatorialEquivalence<Surface>::CombinatorialEquivalence(Predicate predicate): predicate(predicate) {}

template <typename Surface>
std::unique_ptr<EquivalenceClassCode> CombinatorialEquivalence<Surface>::code(const Surface& surface) const {
  // We describe the process for combinatorial equivalence; the general
  // algorithm is described below.

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

  // The procedure for generic codes is similar. We start with some initial
  // "walkers" that emit a code word in each of their steps. We compare these
  // code words and discard any code word that is not minimal und all the
  // minimal walkers are completed. This yields the minimal code for this surface.

  Code code;

  using Walker = CombinatorialWalker;

  std::vector<Walker> walkers = Walker::seedWalkers(surface, predicate);

  std::vector<Walker*> minimalWalkers;

  for (auto& walker: walkers)
    minimalWalkers.push_back(&walker);

  while (true) {
    std::vector<Walker*> stillMinimalWalkers;

    std::optional<Word> minimalWord;

    for (auto* walker : minimalWalkers) {
      const auto word = walker->walk();

      if (!word.has_value())
        continue;

      switch (Walker::cmp(word, minimalWord)) {
        case -1:
          minimalWord = word;
          stillMinimalWalkers = {walker};
          break;
        case 0:
          stillMinimalWalkers.push_back(walker);
          break;
        case 1:
          // We drop this walker. It does not lead to a minimal code (unless all
          // walkers are finished.)
          break;
        default:
          LIBFLATSURF_UNREACHABLE("cmp() must return -1, 0, or 1");
      }

    }

    LIBFLATSURF_ASSERT(stillMinimalWalkers.empty() == !minimalWord.has_value(), "when there is a minimal walker it must have produced a new code word and vice versa");

    if (!minimalWord.has_value())
      // All walkers are done. Our code is complete.
      break;

    Walker::append(code, *minimalWord);

    minimalWalkers = stillMinimalWalkers;
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
CombinatorialEquivalence<Surface>::CombinatorialWalker::CombinatorialWalker(const Surface& surface, HalfEdge start, const Predicate& predicate): surface(&surface), predicate(&predicate) {
  labels[-start] = 0;
  labeled.push_back(-start);
}

template <typename Surface>
std::optional<typename CombinatorialEquivalence<Surface>::Word> CombinatorialEquivalence<Surface>::CombinatorialWalker::walk() {
  if (steps >= labeled.size())
    return std::nullopt;

  Word word;

  const HalfEdge cross = labeled[steps++];

  const HalfEdge start = -cross;

  if (labels.find(start) != std::end(labels))
    // We have processed this face before. This creates an empty word.
    return word;

  // Build the word by walking the face of start.
  HalfEdge pos = start;

  while (true) {
    LIBFLATSURF_ASSERT((*predicate)(*surface, pos), "Cannot record an edge that has been filtered out by the equivalence predicate.");

    pos = surface->nextInFace(pos);

    while (!(*predicate)(*surface, pos))
      pos = surface->previousAtVertex(pos);

    if (pos == start)
      break;

    const int label = this->label(*surface, pos);

    if (label > 0 && label < labels.size() - 2)
      // We found a positive label that was not freshly assigned. We have
      // processed this face before.
      return {};

    word.push_back(label);
  }

  return word;
}

template <typename Surface>
int CombinatorialEquivalence<Surface>::CombinatorialWalker::cmp(const std::optional<Word>& lhs, const std::optional<Word>& rhs) {
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
void CombinatorialEquivalence<Surface>::CombinatorialWalker::append(Code& code, const Word& word) {
  code.push_back(word);
}

template <typename Surface>
int CombinatorialEquivalence<Surface>::CombinatorialWalker::label(const Surface& surface, const HalfEdge halfEdge) {
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

template <typename Surface>
std::vector<typename CombinatorialEquivalence<Surface>::CombinatorialWalker> CombinatorialEquivalence<Surface>::CombinatorialWalker::seedWalkers(const Surface& surface, const Predicate& predicate) {
  std::vector<CombinatorialWalker> walkers;

  for (const auto start : surface.halfEdges()) {
    if (!predicate(surface, start))
      continue;

    walkers.push_back({surface, start, predicate});
  }

  return walkers;
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
