/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2022-2025 Julian Rüth
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

#include <unordered_set>

#include "../../flatsurf/edge.hpp"
#include "../../flatsurf/deformation.hpp"

#include "equivalence_walker.hpp"
#include "linear_equivalence_class_code.hpp"
#include "combinatorial_equivalence_walker.hpp"
#include "linear_equivalence.hpp"

namespace flatsurf {

template <typename Surface>
class LinearEquivalenceWalker : public EquivalenceWalker<Surface, LinearEquivalenceWalker<Surface>> {
 public:
  using T = typename Surface::Coordinate;
  using Character = std::tuple<std::vector<int>, Vector<T>>;
  using Word = typename LinearEquivalenceClassCode<Surface>::Word;
  using Predicate = typename LinearEquivalence<Surface>::Predicate;
  using NormalizationMatrix = typename LinearEquivalence<Surface>::Matrix;
  using Code = LinearEquivalenceClassCode<Surface>;

  LinearEquivalenceWalker(const Surface& surface, HalfEdge start, const NormalizationMatrix& normalization);

  static void append(Word&, const Character&);

  static int cmp(const Character&, const Character&);

  std::optional<Character> step();

  int label(const Surface& surface, const HalfEdge halfEdge);

  // Return the linear normal form of the surface corresponding to the code
  // word this walker found, together with the information whether that surface
  // is a canonical representative for the class.
  Surface normalization() const;

  // Return the deformation that maps the surface to its normalization.
  Deformation<Surface> deformation(const Surface& normalization) const;

  CombinatorialEquivalenceWalker<Surface> combinatorialWalker;

  NormalizationMatrix normalizationMatrix;
};

}

#endif
