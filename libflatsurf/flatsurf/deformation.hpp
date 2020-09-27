/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020 Julian Rüth
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

#include "movable.hpp"

namespace flatsurf {

// The result of deforming a Flat Triangulation.
// Note that currently this is mostly a trivial placeholder interface that we
// will fill with more methods as the need arises.
template <typename Surface>
class Deformation {
  template <typename... Args>
  Deformation(PrivateConstructor, Args&&... args);

 public:
  // Create the identical deformation.
  Deformation(Surface&&);

  // Return the result of the deformation.
  Surface surface();

  // Return the result of the deformation.
  const Surface& surface() const;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const Deformation<S>&);

 private:
  Movable<Deformation> self;

  friend ImplementationOf<Deformation>;
};

}  // namespace flatsurf

#endif
