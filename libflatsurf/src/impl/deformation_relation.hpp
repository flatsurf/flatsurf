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

#ifndef LIBFLATSURF_IMPL_DEFORMATION_RELATION_HPP
#define LIBFLATSURF_IMPL_DEFORMATION_RELATION_HPP

#include <memory>
#include <optional>
#include <ostream>
#include <vector>

#include "read_only.hpp"

namespace flatsurf {

template <typename Surface>
class DeformationRelation {
 public:
  DeformationRelation(const Surface& domain, const Surface& codomain) :
    domain(domain), codomain(codomain) {}
  virtual ~DeformationRelation() {}

  virtual std::unique_ptr<DeformationRelation<Surface>> clone() const = 0;

  virtual std::unique_ptr<DeformationRelation<Surface>> section() const = 0;

  virtual std::optional<Path<Surface>> operator()(const Path<Surface>&) const = 0;

  virtual bool trivial() const;

  virtual std::ostream& operator>>(std::ostream&) const = 0;

  ReadOnly<Surface> domain;
  ReadOnly<Surface> codomain;
};

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const DeformationRelation<Surface>& relation) {
  return relation >> os;
}

}  // namespace flatsurf

#endif
