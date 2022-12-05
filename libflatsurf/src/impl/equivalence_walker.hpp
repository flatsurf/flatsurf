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

namespace flatsurf {

template <typename Surface, typename Walker>
struct EquivalenceWalker {
  EquivalenceWalker(const Surface* surface): surface(surface) {}

  static std::unique_ptr<EquivalenceClassCode> word(std::vector<Walker>&& walkers) {
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

    // Note that things get a bit more complicated here because we support that
    // there is a predicate on the half edges that singles out possibly
    // non-triangular cells that should be the only relevant data for this notion
    // of equivalence.

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

        switch (Walker::cmp(character, minimalCharacter)) {
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

      LIBFLATSURF_ASSERT(stillMinimalWalkers.empty() == !minimalCharacter.has_value(), "when there is a minimal walker it must have produced a new code word and vice versa");

      if (!minimalCharacter.has_value())
        // All walkers are done. Our code is complete.
        break;

      Walker::append(word, *minimalCharacter);

      minimalWalkers = stillMinimalWalkers;
    }

    return std::make_unique<typename Walker::Code>(word);
  }

 protected:
  int steps = 0;
  const Surface* surface;
};

}

#endif
