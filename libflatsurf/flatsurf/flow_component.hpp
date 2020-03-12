/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Julian Rüth
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

#ifndef LIBFLATSURF_COMPONENT_HPP
#define LIBFLATSURF_COMPONENT_HPP

#include <functional>
#include <list>
#include <variant>
#include <vector>

#include <boost/logic/tribool_fwd.hpp>

#include "copyable.hpp"
#include "vector.hpp"

namespace flatsurf {

template <typename Surface>
class FlowComponent {
  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

  using T = typename Surface::Coordinate;

  // Flow components cannot be created directly (other than copying & moving them.)
  // They are created during a FlowDecomposition.
  FlowComponent();

 public:
  // We should be using a FlowPath instead, see https://github.com/flatsurf/flatsurf/issues/146.
  using Perimeter = std::list<FlowConnection<Surface>>;

  boost::logic::tribool cylinder() const;
  boost::logic::tribool withoutPeriodicTrajectory() const;
  boost::logic::tribool keane() const;

  DecompositionStep<Surface> decompositionStep(int limit = -1);

  // Return whether all resulting components satisfy target, i.e., the limit
  // was not reached.
  bool decompose(
      std::function<bool(const FlowComponent&)> target = [](const auto& c) {
        return (c.cylinder() || c.withoutPeriodicTrajectory()) ? true : false;
      },
      int limit = -1);

  // A walk around this component in counter clockwise order along saddle connections.
  Perimeter perimeter() const;

  Surface triangulation() const;

  const IntervalExchangeTransformation<FlatTriangulationCollapsed<T>>& intervalExchangeTransformation() const;

  T width() const;
  T area() const;

  Vertical<Surface> vertical() const;

  bool operator==(const FlowComponent<Surface>&) const;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const FlowComponent<S>&);

 private:
  using Implementation = ImplementationOf<FlowComponent>;
  Copyable<Implementation> impl;

  friend Implementation;
};

}  // namespace flatsurf

#endif
