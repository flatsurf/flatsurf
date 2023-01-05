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


#ifndef LIBFLATSURF_LINEAR_EQUIVALENCE_HPP
#define LIBFLATSURF_LINEAR_EQUIVALENCE_HPP

#include <variant>

#include "../../flatsurf/equivalence.hpp"

#include "equivalence_class_code.hpp"
#include "equivalence.impl.hpp"

namespace flatsurf {

template <typename Surface>
struct LinearEquivalenceWalker;

template <typename Surface>
struct LinearEquivalence : ImplementationOf<Equivalence<Surface>> {
  using T = typename Surface::Coordinate;

  using Predicate = std::function<bool(const Surface&, Edge)>;
  using Matrix = std::tuple<T, T, T, T>;

  enum class GROUP {
    TRIVIAL,  // unlabeled()
    O,  // orthogonal()
    SL,  // areaPreserving()
    GL, // linear()
  };

  using Normalization = std::variant<GROUP, std::function<Matrix(const Surface&, HalfEdge, HalfEdge)>>;

  LinearEquivalence(bool oriented, Normalization normalization);

  std::tuple<std::unique_ptr<EquivalenceClassCode>, std::vector<Deformation<Surface>>> code(const Surface&) const override;
  std::vector<Deformation<Surface>> automorphisms(const Surface&) const override;
  bool equal(const ImplementationOf<Equivalence<Surface>>&) const override;
  std::string toString() const override;

  Matrix normalize(const Surface&, HalfEdge, HalfEdge) const;

  // Return whether the normalization is scaling though it is not allowed to scale.
  // Some normalizations (e.g., orthogonal) are not allowed to scale
  // but still the normalization matrix might perform scaling to work around
  // missing elements in the base ring. In such cases we use this flag to
  // distinguish surfaces that are normalized to the same surface but with
  // different scaling; namely, we add the determinant of the normalization
  // matrix to the code word for the surface.
  bool overscaling() const;

  static Matrix rotate(const Surface&, HalfEdge, HalfEdge);  // implements orthogonal()
  static Matrix orthogonalize(const Surface&, HalfEdge, HalfEdge);  // implements areaPreserving()
  static Matrix orthonormalize(const Surface&, HalfEdge, HalfEdge);  // implements linear()

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const LinearEquivalence<S>&);

 private:
  bool oriented;
  Normalization normalization;
};

}

#endif
