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

#include <intervalxt/forward.hpp>
#include <memory>
#include <ostream>
#include <vector>

#include <fmt/format.h>

#include <boost/type_erasure/any_cast.hpp>

#include <intervalxt/interval_exchange_transformation.hpp>
#include <intervalxt/label.hpp>

// TODO: Remove range-v3/boost ranges everywhere
#include "external/rx-ranges/include/rx/ranges.hpp"

#include "../flatsurf/edge_set.hpp"
#include "../flatsurf/tracked.hpp"
#include "../flatsurf/fmt.hpp"

#include "impl/contour_component.impl.hpp"
#include "impl/flat_triangulation_collapsed.impl.hpp"
#include "impl/interval_exchange_transformation.impl.hpp"
#include "impl/vertical.impl.hpp"

#include "util/assert.ipp"

using rx::to_vector;
using rx::transform;
using std::ostream;
using std::string;
using std::vector;

namespace flatsurf {

template <typename Surface>
IntervalExchangeTransformation<Surface>::IntervalExchangeTransformation(std::shared_ptr<const Surface> surface, const Vector<T>& vertical, const vector<HalfEdge>& top, const vector<HalfEdge>& bottom) :
  impl(spimpl::make_unique_impl<Implementation>(surface, vertical, top, bottom)) {
}

template <typename Surface>
IntervalExchangeTransformation<Surface>::IntervalExchangeTransformation(std::shared_ptr<const Surface> surface, const Vector<T>& vertical, HalfEdge large) :
  impl([&]() {
    CHECK_ARGUMENT(Vertical<Surface>(surface, vertical).large(large), "can only construct IntervalExchangeTransformation from a large half edge");

    vector<HalfEdge> top, bottom;

    // TODO: Can we use ContourComponent directly instead?
    ImplementationOf<ContourComponent<Surface>>::makeContour(back_inserter(top), large, *surface, Vertical(surface, vertical));

    ImplementationOf<ContourComponent<Surface>>::makeContour(back_inserter(bottom), -large, *surface, Vertical(surface, -vertical));
    reverse(bottom.begin(), bottom.end());
    std::transform(bottom.begin(), bottom.end(), bottom.begin(), [](HalfEdge e) { return -e; });
    assert(std::set<HalfEdge>(bottom.begin(), bottom.end()) == std::set<HalfEdge>(top.begin(), top.end()) && "top & bottom contour must contain the same half edges");

    return spimpl::make_unique_impl<Implementation>(surface, vertical, top, bottom);
  }()) {
}

template <typename Surface>
void IntervalExchangeTransformation<Surface>::makeUniqueLargeEdges(Surface& surface, const Vector<T>& vertical_) {
  const typename EdgeSet::FlipHandler nopFlip = [](EdgeSet& self, HalfEdge e) {
    assert(!self.contains(e) && "must not flip source edge");
  };
  const typename EdgeSet::CollapseHandler nopCollapse = [](EdgeSet& self, Edge e) {
    assert(!self.contains(e) && "must not collapse source edge");
  };
  EdgeSet sources(&surface, [](Edge) { return false; }, nopFlip, nopCollapse);

  const bool splitContours = true;

  Vertical<Surface> vertical(surface.shared_from_this(), vertical_);

  while (true) {
    bool stalled = true;
    for (auto source : surface.halfEdges()) {
      if (sources.contains(source))
        continue;
      if (!vertical.large(source))
        continue;
      if (vertical.perpendicular(surface.fromEdge(source)) < 0)
        continue;
      auto component = makeUniqueLargeEdge(surface, vertical_, source);

      if (splitContours) {
        bool trivial, trivialStart, trivialEnd;
        {
          // TODO: There is no LengthAlongTriangulation anymore.
          // Currently, LengthAlongTriangulation cannot handle a collapse.
          // Therefore, we need to scope iet, so it is definitely gone when the
          // collapse below happens.
          auto fromContour = IntervalExchangeTransformation(surface.shared_from_this(), vertical_, source);
          auto& iet = fromContour.intervalExchangeTransformation();
          trivial = iet.top().size() == 1;
          trivialStart = *iet.top().begin() == *iet.bottom().begin();
          trivialEnd = *iet.top().rbegin() == *iet.bottom().rbegin();
        }

        if (!trivial && (trivialStart || trivialEnd)) {
          // Since the first half edge of the top and bottom contour have the
          // same length, flipping the source eventually splits the surface.
          surface.flip(source);
          stalled = false;
          break;
        }
      }

      sources.insert(source);
      stalled = false;
      break;
    }
    if (stalled) {
      return;
    }
  }
}

template <typename Surface>
const intervalxt::IntervalExchangeTransformation& IntervalExchangeTransformation<Surface>::intervalExchangeTransformation() const noexcept { return impl->iet; }

template <typename Surface>
typename intervalxt::IntervalExchangeTransformation& IntervalExchangeTransformation<Surface>::intervalExchangeTransformation() noexcept { return impl->iet; }

template <typename Surface>
std::unordered_set<HalfEdge> IntervalExchangeTransformation<Surface>::makeUniqueLargeEdge(Surface& surface, const Vector<T>& vertical_, HalfEdge& unique_) {
  Tracked<HalfEdge> unique(&surface, HalfEdge(unique_));

  Vertical<Surface> vertical(surface.shared_from_this(), vertical_);

  ASSERT_ARGUMENT(vertical.large(unique), "edge must already be large");
  if (vertical.perpendicular(surface.fromEdge(unique)) < 0)
    unique = -static_cast<HalfEdge>(unique);

  // Eliminate other large edges
  while (true) {
    std::unordered_set<HalfEdge> component;
    if (Vertical<Surface>::Implementation::visit(vertical, unique, component, [&](HalfEdge e) {
          if (e == static_cast<HalfEdge>(unique) || e == -static_cast<HalfEdge>(unique))
            return true;

          if (vertical.large(e)) {
            surface.flip(e);
            return false;
          }

          return true;
        })) {
      assert(component.size() >= 2);
      unique_ = unique;
      return component;
    }
  }
}

template <typename Surface>
Edge IntervalExchangeTransformation<Surface>::edge(const Label& label) const {
  return impl->lengths->fromLabel(label);
}

template <typename Surface>
SaddleConnection<FlatTriangulation<typename Surface::Coordinate>> IntervalExchangeTransformation<Surface>::connection(const intervalxt::Label& label) const {
  return *impl->lengths->lengths.get(impl->lengths->fromLabel(label));
}

template <typename Surface>
ImplementationOf<IntervalExchangeTransformation<Surface>>::ImplementationOf(std::shared_ptr<const Surface> surface, const Vector<T>& vertical, const vector<HalfEdge>& top, const vector<HalfEdge>& bottom) :
  surface(surface) {
  using SaddleConnection = flatsurf::SaddleConnection<FlatTriangulation<T>>;

  const auto uncollapsed = [&]() {
    if constexpr (std::is_same_v<Surface, FlatTriangulation<T>>)
      return surface;
    else
      return surface->uncollapsed();
  }();

  const auto erasedLengths = std::make_shared<intervalxt::Lengths>(Lengths<Surface>(std::make_shared<Vertical<FlatTriangulation<T>>>(uncollapsed, vertical), EdgeMap<std::optional<SaddleConnection>>(surface.get(), [&](const Edge& e) -> std::optional<SaddleConnection> {
    if (std::find(begin(top), end(top), e.positive()) != end(top)) {
      if constexpr (std::is_same_v<Surface, FlatTriangulation<T>>)
        return SaddleConnection(surface, e.positive());
      else
        return surface->fromEdge(e.positive());
    } else if (std::find(begin(top), end(top), e.negative()) != end(top)) {
      if constexpr (std::is_same_v<Surface, FlatTriangulation<T>>)
        return SaddleConnection(surface, e.negative());
      else
        return surface->fromEdge(e.negative());
    }

    return std::optional<SaddleConnection>{};
  })));

  iet = intervalxt::IntervalExchangeTransformation(
      erasedLengths,
      top | transform([&](Edge e) { return intervalxt::Label(e.index()); }) | to_vector(),
      bottom | transform([&](Edge e) { return intervalxt::Label(e.index()); }) | to_vector());

  lengths = boost::type_erasure::any_cast<Lengths<Surface>*>(erasedLengths.get());

  ASSERT(lengths != nullptr, "Setting lengths from erasedLengths should produce the original length type again");

  const auto connected = [&](const vector<HalfEdge>& contour) {
    for (auto it = contour.begin(); it != contour.end() - 1; it++)
      if (Vertex::target(*it, *surface) != Vertex::source(*(it + 1), *surface)) return false;
    return true;
  };

  CHECK_ARGUMENT(std::multiset<HalfEdge>(top.begin(), top.end()) == std::multiset<HalfEdge>(bottom.begin(), bottom.end()), "top and bottom contour must contain the same half edges");
  CHECK_ARGUMENT(connected(top), fmt::format("top contour must be connected but {} is not connected in {}.", fmt::join(top, ", "),*surface));
  CHECK_ARGUMENT(connected(bottom), "bottom contour must be connected");
  ASSERT(std::all_of(begin(top), end(top), [&](Edge e) { return lengths->get(intervalxt::Label(e.index())) > 0; }), "lengths in contour must be positive");
}

template <typename Surface>
void ImplementationOf<IntervalExchangeTransformation<Surface>>::registerDecomposition(const IntervalExchangeTransformation<Surface>& iet, std::shared_ptr<FlowDecompositionState<FlatTriangulation<T>>> state) {
  iet.impl->lengths->registerDecomposition(state);
}

template <typename Surface>
ostream& operator<<(ostream& os, const IntervalExchangeTransformation<Surface>& self) {
  return os << self.intervalExchangeTransformation();
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), IntervalExchangeTransformation, LIBFLATSURF_SURFACE_TYPES)
