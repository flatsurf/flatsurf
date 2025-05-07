/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2025 Julian Rüth
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
#include <iosfwd>
#include <list>
#include <vector>
#include <gmpxx.h>

#include "copyable.hpp"

namespace flatsurf {

// A component of a Flow Decomposition, i.e., a component corresponding to an
// (irreducible) part of an Interval Exchange Transformation.
template <typename Surface>
class FlowComponent : boost::equality_comparable<FlowComponent<Surface>> {
  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type parameter must not have modifiers such as const");

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

  // Return the decomposition from which this component was created.
  FlowDecomposition<Surface> decomposition();

  // Return the decomposition from which this component was created.
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

  std::vector<mpq_class> safInvariant() const;

  const intervalxt::Component& dynamicalComponent() const;

  [[deprecated("This method returns a inconsistent version of the IET from which this component was created originally. Use dynamicalComponent().iet() instead.")]] const IntervalExchangeTransformation<FlatTriangulationCollapsed<T>>& intervalExchangeTransformation() const;

  [[deprecated("This method does not return the actual width of the component, use widthRelative() instead to make this explicit.")]]
  T width() const;

  // Return the width of this component multiplied with the norm of the
  // vertical().
  // Note that for a cylinder the following holds:
  // 2 * widthRelative() * heightRelative() = area2() * norm(vertical())**2.
  T widthRelative() const;

  [[deprecated("This method does not return the actual height of the component, use heightRelative() instead to make this explicit.")]]
  T height() const;

  // Return a lower bound of the relative height of any cylinder contained in
  // this component.
  // If this is a cylinder, return the actual relative height of the cylinder.
  // The relative height is the actual height multiplied with the norm of the
  // vertical().
  // Note that for a cylinder the following holds:
  // 2 * widthRelative() * heightRelative() = area2() * norm(vertical())**2.
  T heightRelative() const;

  [[deprecated("This method returns twice the area. To make this explicit, use area2() instead.")]]
  T area() const;

  // Return twice the area of this component. Note that even for cylinders this
  // is usually not 2 * width() * height().
  T area2() const;

  // Return the vectors going from the non-vertical saddle connections on the
  // bottom to their counterparts on the top; ordered as on the bottom contour
  // from left to right.
  std::vector<Vector<T>> holonomy() const;

  // Return the holonomy of the circumference of this cylinder, i.e., the
  // vector that corresponds the period of this cylinder. Note that this is
  // holonomy()[0].
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
