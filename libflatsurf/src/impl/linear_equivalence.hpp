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


#include "../../flatsurf/equivalence.hpp"
#include "../../flatsurf/equivalence_class_code.hpp"
#include "combinatorial_equivalence.hpp"

#ifndef LIBFLATSURF_LINEAR_EQUIVALENCE_HPP
#define LIBFLATSURF_LINEAR_EQUIVALENCE_HPP

namespace flatsurf {

template <typename Surface>
struct LinearEquivalence : ImplementationOf<Equivalence<Surface>> {
  using T = typename Surface::Coordinate;

  using CombinatorialCode = typename CombinatorialEquivalence<Surface>::Code;
  using Code = std::tuple<CombinatorialCode, std::vector<Vector<T>>>;

  struct LinearEquivalenceClassCode : public EquivalenceClassCode {
    explicit LinearEquivalenceClassCode(Code code);

    size_t hash() const override;
    bool equal(const EquivalenceClassCode&) const override;
    std::string toString() const override;

   private:
    Code code;
  };

  using Predicate = std::function<bool(const Surface&, Edge)>;
  using Normalization = std::function<std::tuple<T, T, T, T>(const Surface&, HalfEdge, HalfEdge)>;

  LinearEquivalence(bool oriented, Normalization normalization, Predicate predicate): oriented(oriented), normalization(normalization), predicate(predicate) {}

  std::unique_ptr<EquivalenceClassCode> code(const Surface&) const override;
  Iterable<Deformation<Surface>> automorphisms() const override;
  void normalize(Surface&) const override;
  bool equal(const ImplementationOf<Equivalence<Surface>>&) const override;
  std::string toString() const override;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const LinearEquivalence<S>&);

 private:
  bool oriented;
  Normalization normalization;
  Predicate predicate;
};

}

#endif

