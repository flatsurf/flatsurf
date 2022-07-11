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
#ifndef LIBFLATSURF_EQUIVALENCE_CLASS_HPP
#define LIBFLATSURF_EQUIVALENCE_CLASS_HPP

#include <boost/operators.hpp>

#include "copyable.hpp"

namespace flatsurf {

// An equivalence class of surfaces.
template <typename T>
class EquivalenceClass : boost::equality_comparable<EquivalenceClass<T>> {
  using Coordinate = T;

 public:
  EquivalenceClass(const FlatTriangulation<T>&, ISOMORPHISM);

  // Return the number of isomorphisms from an element of this equivalence
  // class to an element of the given equivalence class.
  size_t isomorphisms(const EquivalenceClass&) const;

  // Return a representative of this equivalence class.
  const FlatTriangulation<T>& representative() const;

  ISOMORPHISM kind() const;

  // Return whether two equivalence classes are equal.
  // Note that equal equivalence classes might return a different
  // representative.
  friend bool operator==(const EquivalenceClass&, const EquivalenceClass&);

  template <typename S>
  friend std::ostream &operator<<(std::ostream &, const EquivalenceClass<S> &);

 private:
  Copyable<EquivalenceClass> self;

  friend ImplementationOf<EquivalenceClass>;
};

}

template <typename T>
struct std::hash<::flatsurf::EquivalenceClass<T>> {
  size_t operator()(const ::flatsurf::EquivalenceClass<T>&) const;
};

#endif
