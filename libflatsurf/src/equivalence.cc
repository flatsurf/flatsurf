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

#include <ostream>
#include <stdexcept>

#include "../flatsurf/equivalence.hpp"
#include "../flatsurf/edge.hpp"
#include "impl/combinatorial_equivalence.hpp"
#include "impl/linear_equivalence.hpp"
#include "util/assert.ipp"

namespace flatsurf {

template <typename Surface>
Equivalence<Surface> Equivalence<Surface>::combinatorial(std::function<bool(const Surface&, Edge)> predicate) {
  return Equivalence(PrivateConstructor{}, std::make_shared<CombinatorialEquivalence<Surface>>(predicate));
}

template <typename Surface>
Equivalence<Surface> Equivalence<Surface>::unlabeled(std::function<bool(const Surface&, Edge)> predicate) {
  static const auto normalization = std::function([](const Surface&, HalfEdge, HalfEdge) -> std::tuple<T, T, T, T> {
    return {T(1), T(), T(), T(1)};
  });

  return linear(true, normalization, predicate);
}

template <typename Surface>
Equivalence<Surface> Equivalence<Surface>::rotational(std::function<bool(const Surface&, Edge)> predicate) {
  throw std::logic_error("not implemented: Equivalence::rotational()");
}

template <typename Surface>
Equivalence<Surface> Equivalence<Surface>::areaPreserving(bool oriented, std::function<bool(const Surface&, Edge)> predicate) {
  throw std::logic_error("not implemented: Equivalence::areaPreserving()");
}

template <typename Surface>
Equivalence<Surface> Equivalence<Surface>::linear(bool oriented, std::function<Matrix(const Surface&, HalfEdge, HalfEdge)> normalization, std::function<bool(const Surface&, Edge)> predicate) {
  return Equivalence(PrivateConstructor{}, std::make_shared<LinearEquivalence<Surface>>(oriented, normalization, predicate));
}

template <typename Surface>
bool Equivalence<Surface>::all(const Surface&, Edge) {
  return true;
}

template <typename Surface>
bool Equivalence<Surface>::delaunayCell(const Surface&, Edge) {
  throw std::logic_error("not implemented: Equivalence::delaunayEdge()");
}

template <typename Surface>
typename Equivalence<Surface>::Matrix Equivalence<Surface>::orthonormalization(const Surface&, HalfEdge, HalfEdge) {
  throw std::logic_error("not implemented: orthonormalization");
}

template <typename Surface>
ImplementationOf<Equivalence<Surface>>::~ImplementationOf() {}

template <typename Surface>
template <typename Walker>
std::unique_ptr<EquivalenceClassCode> ImplementationOf<Equivalence<Surface>>::code(const std::vector<Walker>& walkers) const {
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

  using Code = typename Walker::Code;
  using Word = typename Walker::Word;

  Code code;

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

  return Walker::code(code);
}

template <typename Surface>
std::ostream& operator<<(std::ostream&, const Equivalence<Surface>& surface) {
  throw std::logic_error("not implemented: operator<< for Equivalence");
}

}

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), Equivalence, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
