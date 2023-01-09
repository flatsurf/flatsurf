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

#ifndef LIBFLATSURF_COMBINATORIAL_EQUIVALENCE_WALKER_HPP
#define LIBFLATSURF_COMBINATORIAL_EQUIVALENCE_WALKER_HPP

#include <unordered_map>

#include "../../flatsurf/half_edge.hpp"
#include "../../flatsurf/deformation.hpp"

#include "equivalence_walker.hpp"
#include "combinatorial_equivalence_class_code.hpp"
#include "combinatorial_equivalence.hpp"

namespace flatsurf {

template <typename Surface>
struct CombinatorialEquivalenceWalker : EquivalenceWalker<Surface, CombinatorialEquivalenceWalker<Surface>> {
  using T = typename Surface::Coordinate;
  using Character = std::vector<int>;
  using Word = CombinatorialEquivalenceClassCode::Word;
  using Code = CombinatorialEquivalenceClassCode;

  CombinatorialEquivalenceWalker(const Surface& surface, HalfEdge start, int orientation);

  static void append(Word&, const Character&);

  static int cmp(const Character&, const Character&);

  std::optional<Character> step();

  int label(const Surface& surface, const HalfEdge halfEdge);

  // Return the combinatorial normal form of the surface that corresponds to
  // the code word this walker found.
  // The vectors underlying this surface are taken from the original surface.
  // Since the normalization has to be a translation surface currently, there
  // is not canonical normal form here. (E.g., by making all faces regular
  // n-gons.)
  Surface normalization() const;

  // Return the deformation that maps the surface to its normalization.
  Deformation<Surface> deformation(const Surface& normalization) const;

  // Return the images of the half edges in the normalization.
  std::unordered_map<HalfEdge, HalfEdge> relabeling() const;

  // Map half edges to 0, 1, 2, … as we encounter them when walking the
  // surface. By construction this will only contain at most a single signed
  // half edge for each edge.
  // (Note that we don't use a HalfEdge map since it is allocating a linear
  // amount of memory from the start which would mean that we'd get quadratic
  // runtime by just initializing linearly many runners.)
  std::unordered_map<HalfEdge, int> labels;

  // The set of labeled half edges in the order they were encountered.
  // Essentially, this is the inverse mapping of labels.
  std::vector<HalfEdge> labeled;

  int orientation;

  int steps = 0;
};

}

#endif
