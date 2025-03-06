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

#ifndef LIBFLATSURF_EQUIVALENCE_IMPL_HPP
#define LIBFLATSURF_EQUIVALENCE_IMPL_HPP

#include <optional>

#include "../../flatsurf/equivalence.hpp"

#include "read_only.hpp"

namespace flatsurf {

template <typename Surface>
class ImplementationOf<Equivalence<Surface>> {
 public:
  virtual ~ImplementationOf();

  // Return a code word that uniquely identifies this surface with respect to
  // this notion of equivalence.
  // Also, returns a (possibly canonical) representative of this surface and a
  // vector of isomorphisms from this surface to the representative.
  virtual std::tuple<std::unique_ptr<EquivalenceClassCode>, ReadOnly<Surface>, std::vector<Deformation<Surface>>> code(const Surface&) const = 0;

  // Return the isomorphisms from ``domain`` to ``codomain`` in this notion of
  // equivalence.
  virtual std::vector<Deformation<Surface>> isomorphisms(const Surface& domain, const Surface& codomain) const;

  // Return whether this notion of equivalence equals ``other``.
  virtual bool equal(const ImplementationOf& other) const = 0;

  // Return a printable representation of this notion of equivalence.
  virtual std::string toString() const = 0;

};

template <typename Surface>
template <typename... Args>
Equivalence<Surface>::Equivalence(PrivateConstructor, Args&&... args) :
  self(args...) {}

}

#endif
