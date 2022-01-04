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

#ifndef LIBFLATSURF_FLOW_DECOMPOSITION_HPP
#define LIBFLATSURF_FLOW_DECOMPOSITION_HPP

#include <boost/logic/tribool.hpp>
#include <boost/operators.hpp>
#include <functional>
#include <iosfwd>
#include <vector>

#include "flow_component.hpp"
#include "movable.hpp"

namespace flatsurf {

// Decomposes a surface into Flow Components with respect to a certain vertical
// direction. Such a decomposition consists of cylinders, minimal components,
// and undetermined components.
template <typename Surface>
class FlowDecomposition : boost::equality_comparable<FlowDecomposition<Surface>> {
  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

  using T = typename Surface::Coordinate;

  template <typename... Args>
  FlowDecomposition(PrivateConstructor, Args&&... args);

 public:
  FlowDecomposition(Surface&&, const Vector<T>& vertical);

  static bool defaultTarget(const FlowComponent<Surface>& c) {
    return (c.cylinder() || c.withoutPeriodicTrajectory()) ? true : false;
  };

  // Return whether all resulting components satisfy target, i.e., target could
  // be established for all components without exceeding the limit.
  bool decompose(
      std::function<bool(const FlowComponent<Surface>&)> target = defaultTarget,
      int limit = -1);

  std::vector<FlowComponent<Surface>> components() const;

  // Return the original surface from which this flow decomposition was created.
  const Surface& surface() const;

  Vertical<Surface> vertical() const;

  // Return a triangulation of surface consistent with the decomposition into flow components.
  FlatTriangulation<T> triangulation() const;

  // Return the half edge in triangulation() corresponding to this flow connection.
  HalfEdge halfEdge(const FlowConnection<Surface>&) const;

  boost::logic::tribool hasCylinder() const;
  boost::logic::tribool completelyPeriodic() const;

  // Return
  //   false: if there is a minimal component or there are two cylinders with incommensurable moduli
  //   true: if all components are cylinders and moduli are commensurable
  //   unknown: otherwise
  boost::logic::tribool parabolic() const;

  // Return whether these flow decompositions are identical.
  bool operator==(const FlowDecomposition<Surface>&) const;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const FlowDecomposition<S>&);

 private:
  Movable<FlowDecomposition> self;

  friend ImplementationOf<FlowDecomposition>;
};

template <typename Surface, typename... Args>
FlowDecomposition(Surface&&, Args&&... args) -> FlowDecomposition<Surface>;

}  // namespace flatsurf

#endif
