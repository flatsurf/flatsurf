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

#ifndef LIBFLATSURF_EQUIVALENCE_CLASS_IMPL_HPP
#define LIBFLATSURF_EQUIVALENCE_CLASS_IMPL_HPP

#include "../../flatsurf/equivalence_class.hpp"
#include "../../flatsurf/equivalence.hpp"
#include "equivalence_class_code.hpp"

#include "read_only.hpp"

namespace flatsurf {

template <typename Surface>
class ImplementationOf<EquivalenceClass<Surface>> {
 public:
  ImplementationOf(const Surface& surface, const Equivalence<Surface>& equivalence);

  ReadOnly<Surface> surface;
  Equivalence<Surface> equivalence;
  std::shared_ptr<EquivalenceClassCode> code;
  size_t automorphisms;
};

}

#endif
