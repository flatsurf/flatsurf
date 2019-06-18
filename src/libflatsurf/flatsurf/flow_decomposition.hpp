/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
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

#include <optional>
#include <vector>
#include "external/spimpl/spimpl.h"

#include "flatsurf/forward.hpp"

namespace flatsurf {

template <typename T>
class Component {
 public:
  using Surface = FlatTriangulation<T>;
  using Boundary = std::vector<const SaddleConnection<Surface>>;

  bool cylinder() const noexcept;
  // Whether this is a cylinder or a certified minimal component.
  bool certified() const noexcept;
  // Maybe circular linked list? (On a cylinder, the right boundary goes with the vertical direction and the left against.)
  const std::vector<Boundary>& boundaries() const noexcept;
  // TODO: It's unclear how to do this exactly.
  FlatTriangulation<T> triangulation() const;
  // The real part of the non-vertical edges of the triangulation()
  LengthAlongTriangulation<T> width() const noexcept;
};

template <class T>
class FlowDecomposition {
 public:
  using Surface = FlatTriangulation<T>;

  FlowDecomposition(const Surface& surface, const Vector<T>& direction);

  // Return the new component if component splits.
  std::optional<const Component<T>&> findSaddleConnection(const Component<T>& component, int limit = -1);

  // TODO: Run findSaddleConnection on every uncertified component recursively.
  void findSaddleConnections(int limit = -1);

  const std::vector<Component<T>>& components() const noexcept;

  const SaddleConnection<Surface>& successor(const SaddleConnection<Surface>&) const;
  // Return the component on the left of this saddle connection, i.e., the
  // component that reports it as its boundary.
  const Component<T>& component(const SaddleConnection<Surface>&) const;

  template <typename S>
  friend std::ostream &operator<<(std::ostream &, const IntervalExchangeTransformation<S> &);

 private:
  class Implementation;
  spimpl::unique_impl_ptr<Implementation> impl;
};
}  // namespace flatsurf

#endif
