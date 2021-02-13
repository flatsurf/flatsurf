/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2020 Julian Rüth
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

#ifndef LIBFLATSURF_FLOW_COMPONENT_HPP
#define LIBFLATSURF_FLOW_COMPONENT_HPP

#include <boost/logic/tribool_fwd.hpp>
#include <boost/operators.hpp>
#include <functional>
#include <intervalxt/forward.hpp>
#include <list>

#include "copyable.hpp"

namespace flatsurf {

// A component of a Flow Decomposition, i.e., a component corresponding to an
// (irreducible) part of an Interval Exchange Transformation.
template <typename Surface>
class FlowComponent : boost::equality_comparable<FlowComponent<Surface>> {
  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

  using T = typename Surface::Coordinate;

  // Flow components cannot be created directly (other than copying & moving them.)
  // They are created during a FlowDecomposition.
  template <typename... Args>
  FlowComponent(PrivateConstructor, Args&&... args);

 public:
  // We should be using a FlowPath instead, see https://github.com/flatsurf/flatsurf/issues/146.
  using Perimeter = std::list<FlowConnection<Surface>>;

  boost::logic::tribool cylinder() const;
  boost::logic::tribool withoutPeriodicTrajectory() const;
  boost::logic::tribool keane() const;

  DecompositionStep<Surface> decompositionStep(int limit = -1);

  FlowDecomposition<Surface> decomposition();

  const FlowDecomposition<Surface> decomposition() const;

  static bool defaultTarget(const FlowComponent<Surface>& c) {
    return (c.cylinder() || c.withoutPeriodicTrajectory()) ? true : false;
  };

  // Return whether all resulting components satisfy target., i.e., the limit
  // was not reached.
  bool decompose(
      std::function<bool(const FlowComponent&)> target = defaultTarget,
      int limit = -1);

  // A walk around this component in counter clockwise order along saddle connections.
  Perimeter perimeter() const;

  Perimeter bottom() const;
  Perimeter right() const;
  Perimeter top() const;
  Perimeter left() const;

  FlowTriangulation<Surface> triangulation() const;

  const intervalxt::Component& dynamicalComponent() const;

  [[deprecated("This method returns a inconsistent version of the IET from which this component was created originally. Use dynamicalComponent().iet() instead.")]] const IntervalExchangeTransformation<FlatTriangulationCollapsed<T>>& intervalExchangeTransformation() const;

  T width() const;

  T area() const;

  // Return the holonomy of the circumference of this cylinder, i.e., the
  // vector that corresponds the period of this cylinder.
  Vector<T> circumferenceHolonomy() const;

  Vertical<Surface> vertical() const;

  bool operator==(const FlowComponent<Surface>&) const;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const FlowComponent<S>&);

 private:
  Copyable<FlowComponent> self;

  friend ImplementationOf<FlowComponent>;
};

}  // namespace flatsurf

#endif
