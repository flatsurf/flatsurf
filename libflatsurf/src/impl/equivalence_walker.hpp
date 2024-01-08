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

#ifndef LIBFLATSURF_EQUIVALENCE_WALKER_HPP
#define LIBFLATSURF_EQUIVALENCE_WALKER_HPP

#include <memory>
#include <vector>
#include <optional>

#include "equivalence_class_code.hpp"

#include "../util/assert.ipp"
#include "read_only.hpp"

namespace flatsurf {

template <typename Surface, typename Walker>
struct EquivalenceWalker {
  EquivalenceWalker(const Surface& surface): surface(surface) {}

  // Return the minimal code word amongst the code words produced by ``walkers``.
  // Also returns a (possibly canonical) normalization of the surface worked on
  // by the ``walkers`` and a vector of isomorphisms to the normalization
  // corresponding to the minimial code words.
  static std::tuple<std::unique_ptr<EquivalenceClassCode>, ReadOnly<Surface>, std::vector<Deformation<Surface>>> word(std::vector<Walker>&& walkers) {
    // We describe the process for combinatorial equivalence; the general
    // algorithm is described below.

    // Pick a random half edge as the starting point and call it half edge 1.
    // Then walk the surface in a breadth-first search, labeling half edges as 2,
    // 3, ... as we find unlabeled ones. The sequence of half edges we see during
    // this walk is a code word for this surface. If we sort the code words and
    // pick the lexicographically largest one, then it uniquely describes this
    // surface combinatorially.

    // To speed things up we compute all the code words in parallel and discard
    // code words that are not the lexicographically largest immediately. As an
    // optimization we don't need to record the label of the edge we are
    // crossing since it just going to be smallest edge label we have seen so
    // far.

    // The procedure for generic code words is similar. We start with some
    // initial "walkers" that emit a code character in each of their steps. We
    // compare these characters and discard any character that is not minimal
    // und all the minimal walkers are completed. This yields the minimal code
    // for this surface.

    using Word = typename Walker::Word;
    using Character = typename Walker::Character;

    Word word;

    std::vector<Walker*> minimalWalkers;

    for (Walker& walker: walkers)
      minimalWalkers.push_back(&walker);

    while (true) {
      std::vector<Walker*> stillMinimalWalkers;

      std::optional<Character> minimalCharacter;

      for (Walker* walker : minimalWalkers) {
        const auto character = walker->step();

        if (!character.has_value())
          continue;

        switch (cmp(character, minimalCharacter)) {
          case -1:
            minimalCharacter = character;
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

      LIBFLATSURF_ASSERT(stillMinimalWalkers.empty() == !minimalCharacter.has_value(), "When there is a minimal walker it must have produced a new code word and vice versa");

      if (!minimalCharacter.has_value())
        // All walkers are done. Our code is complete.
        break;

      Walker::append(word, *minimalCharacter);

      minimalWalkers = stillMinimalWalkers;
    }

    LIBFLATSURF_ASSERT(!minimalWalkers.empty(), "At least one code must be minimal but all were found to be non-minimial.");

    ReadOnly<Surface> normalization = minimalWalkers[0]->normalization();

    std::vector<Deformation<Surface>> deformations;

    for (const auto& walker : minimalWalkers)
      deformations.push_back(walker->deformation(normalization));

    return std::tuple{std::make_unique<typename Walker::Code>(word), normalization, deformations};
  }

  // Compare two code word characters and return -1, 0, 1 depending on how the
  // characters compare.
  template <typename Character>
  static int cmp(const std::optional<Character>& lhs, const std::optional<Character>& rhs) {
    if (!lhs.has_value()) {
      if (!rhs.has_value())
        return 0;
      return 1;
    }

    if (!rhs.has_value())
      return -1;

    return Walker::cmp(*lhs, *rhs);
  }

 protected:
  ReadOnly<Surface> surface;
};

}

#endif
