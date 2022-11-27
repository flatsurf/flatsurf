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

#include "impl/combinatorial_equivalence.hpp"

#include "../flatsurf/equivalence_class_code.hpp"
#include "../flatsurf/iterable.hpp"

namespace flatsurf {

template <typename Surface>
std::unique_ptr<EquivalenceClassCode> CombinatorialEquivalence<Surface>::code(const Surface&) const {
  throw std::logic_error("not implemented: CombinatorialEquivalence::code()");
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
std::ostream& operator<<(std::ostream&, const CombinatorialEquivalence<Surface>&) {
  throw std::logic_error("not implemented: CombinatorialEquivalence ostream operator<<");
}

}

#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), CombinatorialEquivalence, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
