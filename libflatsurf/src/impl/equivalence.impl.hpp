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

#ifndef LIBFLATSURF_EQUIVALENCE_IMPL_HPP
#define LIBFLATSURF_EQUIVALENCE_IMPL_HPP

#include "../../flatsurf/equivalence.hpp"

namespace flatsurf {

template <typename Surface>
struct ImplementationOf<Equivalence<Surface>> {
  virtual ~ImplementationOf();

  // TODO: Document me.
  virtual std::tuple<std::unique_ptr<EquivalenceClassCode>, std::vector<Deformation<Surface>>> code(const Surface&) const = 0;
  // TODO: Document me.
  virtual std::vector<Deformation<Surface>> automorphisms(const Surface&) const = 0;
  // TODO: Document me.
  virtual std::vector<Deformation<Surface>> isomorphisms(const Surface& domain, const Surface& codomain) const;
  // TODO: Document me.
  virtual bool equal(const ImplementationOf&) const = 0;
  // TODO: Document me.
  virtual std::string toString() const = 0;

};

template <typename Surface>
template <typename... Args>
Equivalence<Surface>::Equivalence(PrivateConstructor, Args&&... args) :
  self(args...) {}

}

#endif
