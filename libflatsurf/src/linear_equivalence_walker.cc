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
#include "../flatsurf/edge.hpp"

#include "impl/linear_equivalence_walker.hpp"

namespace flatsurf {

template <typename Surface>
LinearEquivalenceWalker<Surface>::LinearEquivalenceWalker(const Surface* surface, HalfEdge start, const NormalizationMatrix& normalizationMatrix):
  EquivalenceWalker<Surface, LinearEquivalenceWalker>(surface),
  combinatorialWalker(surface, start, std::get<0>(normalizationMatrix) * std::get<3>(normalizationMatrix) - std::get<1>(normalizationMatrix) * std::get<2>(normalizationMatrix) > 0 ? 1 : -1),
  normalizationMatrix(normalizationMatrix)
  {}

template <typename Surface>
void LinearEquivalenceWalker<Surface>::append(Word& word, const Character& character) {
  std::get<0>(word).push_back(std::get<0>(character));
  std::get<1>(word).push_back(std::get<1>(character));
}

template <typename Surface>
int LinearEquivalenceWalker<Surface>::cmp(const Character& lhs, const Character& rhs) {

  int cmp = CombinatorialEquivalenceWalker<Surface>::cmp(std::get<0>(lhs), std::get<0>(rhs));

  if (cmp != 0)
    return cmp;

  const auto& lvector = std::get<1>(lhs);
  const auto& rvector = std::get<1>(rhs);

  if (lvector.x() < rvector.x())
    return -1;
  if (lvector.x() > rvector.x())
    return 1;

  if (lvector.y() < rvector.y())
    return -1;
  if (lvector.y() > rvector.y())
    return 1;

  return 0;
}

template <typename Surface>
std::optional<typename LinearEquivalenceWalker<Surface>::Character> LinearEquivalenceWalker<Surface>::step() {
  const auto combinatorial = combinatorialWalker.step();

  if (!combinatorial)
    return std::nullopt;

  const HalfEdge crossed = combinatorialWalker.labeled[combinatorialWalker.steps-1];
  const auto normalized = this->surface->fromHalfEdge(crossed).applyMatrix(
    std::get<0>(this->normalizationMatrix),
    std::get<1>(this->normalizationMatrix),
    std::get<2>(this->normalizationMatrix),
    std::get<3>(this->normalizationMatrix));

  return std::tuple{*combinatorial, normalized};
}

template <typename Surface>
Deformation<Surface> LinearEquivalenceWalker<Surface>::deformation(const Surface& codomain) const {
  // TODO: Implement me.
  throw std::logic_error("not implemented: deformation()");
}

template <typename Surface>
Surface LinearEquivalenceWalker<Surface>::normalization() const {
  throw std::logic_error("not implemented: normalization()");
}

}


#include "../flatsurf/vector.hpp"
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_STATIC), LinearEquivalenceWalker, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
