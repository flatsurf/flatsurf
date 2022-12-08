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
#ifndef LIBFLATSURF_EQUIVALENCE_HPP
#define LIBFLATSURF_EQUIVALENCE_HPP

#include <boost/operators.hpp>

#include <type_traits>

#include <tuple>
#include <any>
#include <functional>
#include <memory>

#include "shared.hpp"

namespace flatsurf {

// A notion of equality of surfaces.
template <typename Surface>
class Equivalence : boost::equality_comparable<Equivalence<Surface>> {
  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type parameter must not have modifiers such as const");
  using T = typename Surface::Coordinate;
  using Matrix = std::tuple<T, T, T, T>;

  // Equivalences cannot be created directly.
  // Use the static members to create equivalences.
  template <typename... Args>
  Equivalence(PrivateConstructor, Args&&... args);

 public:
  // Return the combinatorial equivalence of surfaces on marked edges.
  // The edges singled out by the predicate must be such that the graph they
  // form is connected and has no cut-edges. In other words, the surface is
  // such that we can walk the boundaries of the (not necessarily triangular)
  // faces; the default is to pick all edges.
  // Two surfaces are then equivalent if after relabeling they have the same
  // sequence of half edges along their faces.
  // If oriented is False, then the relabeling may be such that it reverses the
  // order of half edges around vertices everywhere. The default is to only
  // consider relabelings that preserve orientation.
  static Equivalence combinatorial(bool oriented = true, std::function<bool(const Surface&, Edge)> = nullptr);

  // Return the equivalence of surfaces modulo labeling on marked edges.
  // The edges singled out by the predicate must be such that the graph they
  // form is connected and has no cut-edges. In other words, the surface is
  // such that we can walk the boundaries of the (not necessarily triangular)
  // faces; the default is to pick all edges.
  // Two surfaces are then equivalent if half edges can be relabeled such that
  // the selected faces are identical, i.e., they are combinatorially
  // equivalent and also assign the same vector to each half edge label.
  static Equivalence unlabeled(std::function<bool(const Surface&, Edge)> = nullptr);

  // Return the rotational equivalence of surfaces on marked edges.
  // The edges singled out by the predicate must be such that the graph they
  // form is connected and has no cut-edges. In other words, the surface is
  // such that we can walk the boundaries of the (not necessarily triangular)
  // faces; the default is to pick all edges.
  // Two surfaces are then equivalent if the half edges can be relabeled such
  // that the selected faces are identical module a fixed element of the
  // special orthogonal group.
  // If oriented is False, then we also allow transformations of negative
  // determinant; the default is to only allow rotations.
  static Equivalence orthogonal(bool oriented = true, std::function<bool(const Surface&, Edge)> = nullptr);

  // Return the SL2 equivalence of surfaces on marked edges.
  // The edges singled out by the predicate must be such that the graph they
  // form is connected and has no cut-edges. In other words, the surface is
  // such that we can walk the boundaries of the (not necessarily triangular)
  // faces; the default is to pick all edges.
  // Two surfaces are then equivalent if the half edges can be relabeled such
  // that the selected faces are identical module a fixed transformation in
  // SL2(R) or SL2±(R) if oriented is not set.
  static Equivalence areaPreserving(bool oriented = true, std::function<bool(const Surface&, Edge)> = nullptr);

  // Return the GL2 equivalence of surfaces on marked edges.
  // The edges singled out by the predicate must be such that the graph they
  // form is connected and has no cut-edges. In other words, the surface is
  // such that we can walk the boundaries of the (not necessarily triangular)
  // faces; the default is to pick all edges.
  // Two surfaces are then equivalent if the half edges can be relabeled such
  // that the selected faces are identical module a fixed transformation.
  // Candidates for that transformation are produced by the provided
  // normalization. Namely, given two half edges that are counterclockwise
  // consecutive at a vertex, the normalization produces a linear
  // transformation that brings these half edges into a normal form. The
  // default is to make the edges orthonormal.
  // If oriented is False, then transformation with negative determinant are
  // also considered, i.e., the half edges presented to the normalization are
  // also clockwise consecutive at a vertex.
  static Equivalence linear(bool oriented = true, std::function<Matrix(const Surface&, HalfEdge, HalfEdge)> normalization = nullptr, std::function<bool(const Surface&, Edge)> = nullptr);

  // Return whether these notions of equivalence are identical.
  // Note that equivalences that were created with a custom predicate or other
  // callable will be compared by their memory address.
  bool operator==(const Equivalence& other) const;

  template <typename S>
  friend std::ostream &operator<<(std::ostream &, const Equivalence<S> &);

 private:
  Shared<Equivalence> self;

  friend ImplementationOf<Equivalence>;
  friend ImplementationOf<EquivalenceClass<Surface>>;
};

}

#endif
