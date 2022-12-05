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

#include "impl/linear_equivalence_walker.hpp"

namespace flatsurf {

template <typename Surface>
LinearEquivalenceWalker<Surface>::LinearEquivalenceWalker(const Surface* surface, HalfEdge start, const Predicate* predicate, const Normalization& normalization): EquivalenceWalker<Surface, LinearEquivalenceWalker>(surface) {
  throw std::logic_error("not implemented: LinearEquivalenceWalker()");
}

template <typename Surface>
void LinearEquivalenceWalker<Surface>::append(Word&, const Character&) {
  throw std::logic_error("not implemented: LinearEquivalenceWalker::append()");
}

template <typename Surface>
int LinearEquivalenceWalker<Surface>::cmp(const std::optional<Character>&, const std::optional<Character>&) {
  throw std::logic_error("not implemented: LinearEquivalenceWalker::cmp()");
}

template <typename Surface>
std::optional<typename LinearEquivalenceWalker<Surface>::Character> LinearEquivalenceWalker<Surface>::step() {
  throw std::logic_error("not implemented: LinearEquivalenceWalker::step()");
}

}


#include "../flatsurf/vector.hpp"
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_STATIC), LinearEquivalenceWalker, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
