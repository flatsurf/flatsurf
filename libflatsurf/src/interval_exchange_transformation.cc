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

#include <fmt/format.h>

#include <boost/type_erasure/any_cast.hpp>
#include <intervalxt/forward.hpp>
#include <intervalxt/interval_exchange_transformation.hpp>
#include <intervalxt/label.hpp>
#include <memory>
#include <ostream>
#include <unordered_set>
#include <vector>

#include "../flatsurf/edge_set.hpp"
#include "../flatsurf/fmt.hpp"
#include "../flatsurf/tracked.hpp"
#include "../flatsurf/vertex.hpp"
#include "external/rx-ranges/include/rx/ranges.hpp"
#include "impl/asserted_lengths.hpp"
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

    ImplementationOf<ContourComponent<Surface>>::makeContour(back_inserter(top), large, *surface, Vertical(surface, vertical));

    ImplementationOf<ContourComponent<Surface>>::makeContour(back_inserter(bottom), -large, *surface, Vertical(surface, -vertical));
    reverse(bottom.begin(), bottom.end());
    std::transform(bottom.begin(), bottom.end(), bottom.begin(), [](HalfEdge e) { return -e; });
    assert(std::unordered_set<HalfEdge>(bottom.begin(), bottom.end()) == std::unordered_set<HalfEdge>(top.begin(), top.end()) && "top & bottom contour must contain the same half edges");

    return spimpl::make_unique_impl<Implementation>(surface, vertical, top, bottom);
  }()) {
}

template <typename Surface>
void IntervalExchangeTransformation<Surface>::makeUniqueLargeEdges(Surface& surface, const Vector<T>& vertical_) {
  Tracked<EdgeSet> sources(
      &surface, EdgeSet(), [](auto& sources, const auto&, HalfEdge e) { ASSERT(!sources.contains(e), "Selected source edges cannot be flipped."); }, [](auto& sources, const auto&, Edge e) { ASSERT(!sources.contains(e), "Selected source edges cannot be collapsed."); }, [](auto& sources, const auto& surface, HalfEdge a, HalfEdge b) { Tracked<EdgeSet>::defaultSwap(sources, surface, a, b); }, [](auto& sources, const auto& surface, const auto& edges) {
    ASSERT(edges | rx::all_of([&](Edge e) { return !sources.contains(e); }), "Selected source edges cannot be erased.");
    Tracked<EdgeSet>::defaultErase(sources, surface, edges); });

  const bool splitContours = true;

  Vertical<Surface> vertical(surface.shared_from_this(), vertical_);

  while (true) {
    auto larges = surface.halfEdges() | rx::filter([&](const HalfEdge source) {
      if (sources->contains(source)) return false;
      if (!vertical.large(source)) return false;
      if (vertical.perpendicular(surface.fromEdge(source)) < 0) return false;
      return true;
    }) | rx::to_vector();

    std::sort(begin(larges), end(larges), [&](const HalfEdge a, const HalfEdge b) {
      auto alength = vertical.horizontal() * surface.fromEdge(a);
      auto blength = vertical.horizontal() * surface.fromEdge(b);

      if (alength < 0) alength = -alength;
      if (blength < 0) blength = -blength;

      return alength < blength;
    });

    if (larges.size() == 0) break;

    HalfEdge source = larges.back();

    auto component = makeUniqueLargeEdge(surface, vertical_, source);

    if (splitContours) {
      bool trivial, trivialStart, trivialEnd;
      {
        // Some attached data might not be able to handle the following
        // collapspe.  Therefore, we need to scope iet, so attached data is
        // definitely gone when the collapse below happens.
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
        continue;
      }
    }

    sources->insert(source);
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

  // Collect the half edges connected to `unique`, the unique large edge in
  // its component. Whenever a flip is performed, the process restarts.
  while (true) {
    std::unordered_set<HalfEdge> component;
    if (ImplementationOf<Vertical<Surface>>::visit(vertical, unique, component, [&](HalfEdge e) {
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
const SaddleConnection<FlatTriangulation<typename Surface::Coordinate>>& IntervalExchangeTransformation<Surface>::operator[](const intervalxt::Label& label) const {
  return *impl->lengths->lengths[impl->lengths->fromLabel(label)];
}

template <typename Surface>
intervalxt::IntervalExchangeTransformation IntervalExchangeTransformation<Surface>::clone() const {
  auto forgottenLengths = std::make_shared<intervalxt::Lengths>(impl->lengths->forget());
  return intervalxt::IntervalExchangeTransformation(forgottenLengths, impl->iet.top(), impl->iet.bottom());
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

  auto nonzerolengths = EdgeMap<std::optional<SaddleConnection>>(*surface);
  for (auto e : top) {
    if constexpr (std::is_same_v<Surface, FlatTriangulation<T>>)
      nonzerolengths[e] = SaddleConnection(surface, e);
    else
      nonzerolengths[e] = surface->fromEdge(e);
  }

  const auto erasedLengths = std::make_shared<intervalxt::Lengths>(AssertedLengths<Surface>(std::make_shared<Vertical<FlatTriangulation<T>>>(uncollapsed, vertical), std::move(nonzerolengths)));

  iet = intervalxt::IntervalExchangeTransformation(
      erasedLengths,
      top | transform([&](Edge e) { return intervalxt::Label(e.index()); }) | to_vector(),
      bottom | transform([&](Edge e) { return intervalxt::Label(e.index()); }) | to_vector());

  lengths = &boost::type_erasure::any_cast<AssertedLengths<Surface>*>(erasedLengths.get())->lengths;

  ASSERT(lengths != nullptr, "Setting lengths from erasedLengths should produce the original length type again");

  const auto connected = [&](const vector<HalfEdge>& contour) {
    for (auto it = contour.begin(); it != contour.end() - 1; it++)
      if (Vertex::target(*it, *surface) != Vertex::source(*(it + 1), *surface)) return false;
    return true;
  };

  CHECK_ARGUMENT(std::unordered_multiset<HalfEdge>(top.begin(), top.end()) == std::unordered_multiset<HalfEdge>(bottom.begin(), bottom.end()), "top and bottom contour must contain the same half edges");
  CHECK_ARGUMENT(connected(top), fmt::format("top contour must be connected but {} is not connected in {}.", fmt::join(top, ", "), *surface));
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
