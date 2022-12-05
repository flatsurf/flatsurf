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

#ifndef LIBFLATSURF_LINEAR_EQUIVALENCE_WALKER_HPP
#define LIBFLATSURF_LINEAR_EQUIVALENCE_WALKER_HPP

#include "equivalence_walker.hpp"
#include "linear_equivalence_class_code.hpp"
#include "linear_equivalence.hpp"

namespace flatsurf {

template <typename Surface>
struct LinearEquivalenceWalker : EquivalenceWalker<Surface, LinearEquivalenceWalker<Surface>> {
  using Character = std::vector<int>;
  using Word = typename LinearEquivalenceClassCode<Surface>::Word;
  using Predicate = typename LinearEquivalence<Surface>::Predicate;
  using Normalization = typename LinearEquivalence<Surface>::Matrix;
  using Code = LinearEquivalenceClassCode<Surface>;

  LinearEquivalenceWalker(const Surface* surface, HalfEdge start, const Predicate* predicate, const Normalization& normalization);

  static void append(Word&, const Character&);

  static int cmp(const std::optional<Character>&, const std::optional<Character>&);

  std::optional<Character> step();

  int label(const Surface& surface, const HalfEdge halfEdge);

  const Predicate* predicate;
};

}

#endif

