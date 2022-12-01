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

namespace flatsurf {

template <typename Surface>
Equivalence<Surface>::Equivalence(std::shared_ptr<ImplementationOf<Equivalence>> impl) : self(impl) {}

template <typename Surface>
Equivalence<Surface> Equivalence<Surface>::combinatorial(std::function<bool(const Surface&, Edge)> predicate) {
  return Equivalence(std::make_unique<CombinatorialEquivalence<Surface>>(predicate));
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
Equivalence<Surface> Equivalence<Surface>::linear(bool oriented, std::function<std::tuple<T, T, T, T>(const Surface&, HalfEdge, HalfEdge)> normalization, std::function<bool(const Surface&, Edge)>) {
  throw std::logic_error("not implemented: Equivalence::linear()");
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
ImplementationOf<Equivalence<Surface>>::~ImplementationOf() {}

template <typename Surface>
std::ostream& operator<<(std::ostream&, const Equivalence<Surface>& surface) {
  throw std::logic_error("not implemented: operator<< for Equivalence");
}

}

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), Equivalence, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
