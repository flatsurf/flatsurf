/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2021 Julian Rüth
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

#ifndef LIBFLATSURF_DEFORMATION_HPP
#define LIBFLATSURF_DEFORMATION_HPP

#include <iosfwd>
#include <optional>
#include <vector>

#include "movable.hpp"

namespace flatsurf {

// The result of deforming a Flat Triangulation.
template <typename Surface>
class Deformation {
  template <typename... Args>
  Deformation(PrivateConstructor, Args&&... args);

 public:
  using Coordinate = typename Surface::Coordinate;

  // Create the identical deformation.
  [[deprecated("Create from const Surface& instead.")]] explicit Deformation(Surface&&);

  // Create the identical deformation.
  explicit Deformation(const Surface&);

  // Create a copy of this deformation.
  Deformation(const Deformation&);

  Deformation(Deformation&&);

  // Return the image of the half edge under the deformation.
  [[deprecated("Use operator(Path) instead")]] std::optional<HalfEdge> operator()(HalfEdge) const;

  // Return the image of a path under this deformation.
  // In general the returned value is only correct modulo homotopy (relative to
  // the start and end point of the path.)
  // Return nothing if there is no such image, e.g., because the deformation is
  // not [total]().
  std::optional<Path<Surface>> operator()(const Path<Surface>&) const;

  // Return the result of the deformation.
  [[deprecated("Use codomain().clone() instead.")]] Surface surface();

  // Return the result of the deformation.
  [[deprecated("Use codomain() instead.")]] const Surface& surface() const;

  const Surface& domain() const;

  const Surface& codomain() const;

  Deformation& operator=(const Deformation&);

  Deformation& operator=(Deformation&&);

  // Return whether this is a total map of topological spaces.
  bool total() const;

  // Return whether this is a (total) continuous map of topological spaces.
  bool continuous() const;

  // Return whether this is a homeomorphism of topological spaces.
  // This is equivalent to `continuous() && section().continuous()`.
  bool homeomorphic() const;

  // Return whether this is an (total but probably not surjective) isometry for
  // the usual metric on the surfaces.
  bool isometric() const;

  // Return whether this is a retriangulation.
  // If `marked` is set, return whether this deformation can be obtained by
  // performing a sequence of half edge flips.
  // Otherwise, return whether this deformation can be obtained by performing a
  // sequence of flips, and addition and removal of marked points.
  bool retriangulating(bool marked = false) const;

  // Return whether this is a reflection map.
  // Return whether this deformation can be obtained by applying a matrix of
  // determinant -1 to the domain and then retriangulating; marked points are
  // taken into account if `marked` is set, see [retriangulation]().
  bool reflecting(bool marked = false) const;

  // Return whether this is a rotation map.
  // Return whether this deformation can be obtained by applying a rotation
  // matrix to the domain and then retriangulating; marked points are taken
  // into account if `marked` is set, see [retriangulation]().
  bool rotating(bool marked = false) const;

  // Return whether this is a linear transformation.
  // Return whether this deformation can be obtained by applying a linear
  // transformation to the domain and then retriangulating; marked points are
  // taken into account if `marked` is set, see [retriangulation]().
  bool linear(bool marked = false) const;

  // Return whether this is a relabeling of half edges.
  bool relabeling() const;

  // Return whether this is the trivial deformation.
  // Return whether this deformation is entirely trivial, i.e., mapping
  // trivially between indistinguishable triangulations using the same labels.
  bool trivial() const;

  // Return the concatenation of this deformation and `rhs`.
  // The returned deformation corresponds to first applying `rhs` and then this
  // deformation.
  Deformation operator*(const Deformation& rhs) const;

  // Return a (partial) section of this deformation.
  Deformation section() const;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const Deformation<S>&);

 private:
  Movable<Deformation> self;

  friend ImplementationOf<Deformation>;
};

}  // namespace flatsurf

#endif
