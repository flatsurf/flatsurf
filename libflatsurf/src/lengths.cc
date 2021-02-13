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

#include "impl/lengths.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <exact-real/element.hpp>
#include <exact-real/integer_ring.hpp>
#include <exact-real/module.hpp>
#include <exact-real/number_field.hpp>
#include <exact-real/rational_field.hpp>
#include <gmpxxll/mpz_class.hpp>
#include <intervalxt/interval_exchange_transformation.hpp>
#include <intervalxt/length.hpp>
#include <intervalxt/sample/element_coefficients.hpp>
#include <intervalxt/sample/element_floor_division.hpp>
#include <intervalxt/sample/integer_coefficients.hpp>
#include <intervalxt/sample/integer_floor_division.hpp>
#include <intervalxt/sample/lengths.hpp>
#include <intervalxt/sample/mpq_coefficients.hpp>
#include <intervalxt/sample/mpq_floor_division.hpp>
#include <intervalxt/sample/mpz_coefficients.hpp>
#include <intervalxt/sample/mpz_floor_division.hpp>
#include <intervalxt/sample/renf_elem_coefficients.hpp>
#include <intervalxt/sample/renf_elem_floor_division.hpp>
#include <ostream>

#include "../flatsurf/ccw.hpp"
#include "../flatsurf/chain.hpp"
#include "../flatsurf/interval_exchange_transformation.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/vertical.hpp"
#include "external/rx-ranges/include/rx/ranges.hpp"
#include "impl/assert_connection.hpp"
#include "impl/flow_component.impl.hpp"
#include "impl/flow_connection.impl.hpp"
#include "impl/saddle_connection.impl.hpp"
#include "util/assert.ipp"
#include "util/false.ipp"

namespace flatsurf {

using fmt::format;
using intervalxt::Label;
using intervalxt::Length;
using rx::none_of;

template <typename Surface>
Lengths<Surface>::Lengths(const Vertical<FlatTriangulation<T>>& vertical, EdgeMap<std::optional<SaddleConnection<FlatTriangulation<T>>>>&& lengths) :
  vertical(vertical),
  lengths(std::move(lengths)),
  stack(),
  sum() {
  this->lengths.apply([&](const auto& edge, const auto& connection) {
    LIBFLATSURF_CHECK_ARGUMENT(!connection || vertical.ccw(*connection) == CCW::CLOCKWISE, "nontrivial length must be positive but " << edge << " is " << *connection);
  });
}

template <typename Surface>
void Lengths<Surface>::push(Label label) {
  LIBFLATSURF_ASSERT(stack | none_of([&](const auto& l) { return l == label; }), "must not push the same label twice");
  stack.push_back(label);
  sum += length(label);
}

template <typename Surface>
void Lengths<Surface>::pop() {
  LIBFLATSURF_ASSERT(not stack.empty(), "cannot pop from an empty stack");
  sum -= length(stack.back());
  stack.pop_back();
  LIBFLATSURF_ASSERT(!stack.empty() || sum == T(), "sum inconsistent with stack");
}

template <typename Surface>
FlowComponentState<FlatTriangulation<typename Surface::Coordinate>>& Lengths<Surface>::component(Label label) const {
  auto _state = state.lock();
  const auto component = std::find_if(begin(_state->components), end(_state->components), [&](const auto& component) {
    const auto firstTop = static_cast<::intervalxt::Label>(*begin(component.dynamicalComponent.topContour()));
    const auto firstBottom = static_cast<::intervalxt::Label>(*begin(component.dynamicalComponent.bottomContour()));
    return firstTop == label || firstBottom == label;
  });
  LIBFLATSURF_ASSERT(component != end(_state->components), "Label not at the left end of a flow component.")
  return *component;
}

template <typename Surface>
bool Lengths<Surface>::minuendOnTop(Label minuend) const {
  const auto& component = this->component(minuend);
  bool minuendOnTop = static_cast<::intervalxt::Label>(*begin(component.dynamicalComponent.topContour())) == minuend;
  LIBFLATSURF_ASSERT(minuendOnTop || static_cast<::intervalxt::Label>(*begin(component.dynamicalComponent.bottomContour())) == minuend, "minuend not found on any contour");
  return minuendOnTop;
}

template <typename Surface>
void Lengths<Surface>::subtract(Label minuend) {
  subtractRepeated(minuend, 1);
}

template <typename Surface>
Label Lengths<Surface>::subtractRepeated(Label minuend) {
  const auto& component = this->component(minuend);
  const bool minuendOnTop = this->minuendOnTop(minuend);
  const auto subtrahendContour = minuendOnTop ? component.dynamicalComponent.bottomContour() : component.dynamicalComponent.topContour();
  const auto bottomMinuend = std::find_if(begin(subtrahendContour), end(subtrahendContour), [&](const auto& he) { return static_cast<Label>(he) == minuend; });

  LIBFLATSURF_ASSERT(bottomMinuend != end(subtrahendContour), "each label must be in the top and bottom contour but minuend only found in the top contour");

  const bool stableCombinatorics = bottomMinuend->left().size() == 0;

  auto ret = stack.back();
  if (stableCombinatorics) {
    const auto quotient = ::intervalxt::sample::FloorDivision<T>()(length(minuend), length());
    mpz_class iterations = gmpxxll::mpz_class(quotient);
    if (quotient * length() == length(minuend))
      iterations -= 1;
    LIBFLATSURF_ASSERT(iterations > mpz_class(), "subtractRepeated() should not be called when there is no full subtract possible; but the labels on the stack fit only " << iterations << " times into the minuend label " << render(minuend) << "; the code cannot handle partial subtracts yet.");
    subtractRepeated(minuend, iterations);
  } else {
    subtractRepeated(minuend, 1);
  }

  return ret;
}

template <typename Surface>
void Lengths<Surface>::subtractRepeated(Label minuend, const mpz_class& iterations) {
  LIBFLATSURF_ASSERT(iterations > 0, "must subtract at least once");
  LIBFLATSURF_ASSERT(length(minuend) > 0, "lengths must be positive");

  const auto expected = [&]() -> T {
    if constexpr (std::is_same_v<T, long long>)
      return length(minuend) - iterations.get_ui() * length();
    else
      return length(minuend) - iterations * length();
  }();
  LIBFLATSURF_ASSERT(expected > 0, "Lengths must be positive but subtracting " << length() << " " << iterations << " times from edge " << fromLabel(minuend) << " of length " << length(minuend) << " would yield " << expected << " which is non-positive.");
  LIBFLATSURF_ASSERT(expected < length(minuend), "subtraction must shorten lengths");

  auto& component = this->component(minuend);

  const bool minuendOnTop = this->minuendOnTop(minuend);

  const auto minuendContour = minuendOnTop ? component.dynamicalComponent.topContour() : component.dynamicalComponent.bottomContour();
  const auto subtrahendContour = minuendOnTop ? component.dynamicalComponent.bottomContour() : component.dynamicalComponent.topContour();

  const auto flow = [&](const auto& connections, bool reverse) {
    auto flowed = connections | rx::transform([&](const auto& connection) {
      auto _state = state.lock();
      return ImplementationOf<FlowConnection<FlatTriangulation<T>>>::make(_state, ImplementationOf<FlowComponent<FlatTriangulation<T>>>::make(_state, &component), connection).saddleConnection();
    }) | rx::transform([&](const auto& connection) {
      return reverse ? -connection : connection;
    }) | rx::to_vector();
    if (reverse)
      std::reverse(begin(flowed), end(flowed));
    return flowed;
  };

  auto& minuendConnection = lengths[fromLabel(minuend)];

  {
    minuendConnection = -*minuendConnection;

    HalfEdge target = minuendConnection->target();
    Chain walk(minuendConnection->surface());

    // Walk down on the minuend's (top) left boundary
    {
      // left() is oriented from bottom to top so we need to reverse. This should probably be changed.
      for (const auto& connection : flow(begin(minuendContour)->left() | rx::reverse() | rx::to_vector(), !minuendOnTop)) {
        walk += connection;
        target = connection.target();
      }
    }

    // Walk across every subtrahend
    {
      for (const auto& subtrahend : subtrahendContour) {
        LIBFLATSURF_ASSERT(subtrahend != *rbegin(subtrahendContour), "Stack cannot contain every label in the IntervalExchangeTransformation when subtracting.");

        for (const auto& connection : flow(subtrahend.cross(), !minuendOnTop)) {
          walk += connection;
          target = connection.target();
        }

        if (static_cast<::intervalxt::Label>(subtrahend) == stack.back())
          break;
      }
    }

    // If we want to subtract this gadget more than once, we just need to scale
    // the chain; source/target are not affected by this. (They might change,
    // but they are still in the same half plane of the same vertex.)
    // Note that this is more complicated when the bottom minuend has
    // connections on its left. Therefore we need the caller to do a simple
    // subtract before doing a subtractRepeated.
    walk *= iterations;

    auto vector = walk + *minuendConnection;

    // The tricky part is now to figure out the actual source/target sector of
    // the updated SaddleConnection.
    // This should probably be moved into SaddleConnection at some point.

    LIBFLATSURF_ASSERT(vertical->ccw(vector) == CCW::COUNTERCLOCKWISE, "Length must be positive.");

    // We know that minuendConnection (which we reversed in the beginning) is
    // pointing left and that the new -minuendConnection must point left into
    // the same half plane.
    auto source = minuendConnection->source();
    while (vertical->ccw(source) == CCW::COUNTERCLOCKWISE)
      source = minuendConnection->surface().previousAtVertex(source);
    while (!minuendConnection->surface().inSector(source, vector))
      source = minuendConnection->surface().nextAtVertex(source);

    while (vertical->ccw(target) != CCW::CLOCKWISE)
      target = minuendConnection->surface().previousAtVertex(target);
    while (!minuendConnection->surface().inSector(target, -vector))
      target = minuendConnection->surface().nextAtVertex(target);

    minuendConnection = SaddleConnection<FlatTriangulation<T>>(minuendConnection->surface(), target, source, -vector);

    LIBFLATSURF_ASSERT(minuendConnection->source() == target && minuendConnection->target() == source, "We tried to get SaddleConnection source/target right but SaddleConnection constructor disagrees.");

    LIBFLATSURF_ASSERTIONS(([&]() {
      static thread_local AssertConnection<T> assertion;

      LIBFLATSURF_ASSERT(assertion(*minuendConnection), "Connection after subtract does not actually exist in surface. There is no saddle connection " << *minuendConnection << " in " << minuendConnection->surface());
    }));
  }

  LIBFLATSURF_ASSERT(get(minuend), "lengths must be non-zero");
  LIBFLATSURF_ASSERT(length(minuend) == expected, "subtract inconsistent: subtracted " << length() << " from " << fromLabel(minuend) << " which should have yielded " << expected << " but got " << length(minuend) << " instead");

  stack.clear();
  sum = T();
}

template <typename Surface>
Lengths<Surface>::Lengths(const Lengths<Surface>& lengths, std::shared_ptr<FlowDecompositionState<FlatTriangulation<T>>> state) :
  state(state),
  vertical(lengths.vertical),
  lengths(lengths.lengths),
  stack(lengths.stack),
  sum(lengths.sum) {}

template <typename Surface>
std::vector<std::vector<mpq_class>> Lengths<Surface>::coefficients(const std::vector<Label>& labels) const {
  return intervalxt::sample::Coefficients<T>()(labels | rx::transform([&](const Label& label) { return length(label); }) | rx::to_vector());
}

template <typename Surface>
int Lengths<Surface>::cmp(Label rhs) const {
  if (length() < length(rhs))
    return -1;
  else if (length() > length(rhs))
    return 1;
  return 0;
}

template <typename Surface>
int Lengths<Surface>::cmp(Label lhs, Label rhs) const {
  if (length(lhs) < length(rhs))
    return -1;
  else if (length(lhs) > length(rhs))
    return 1;
  return 0;
}

template <typename Surface>
typename Surface::Coordinate Lengths<Surface>::get(Label label) const {
  return length(label);
}

template <typename Surface>
std::string Lengths<Surface>::render(Label label) const {
  const size_t index = std::hash<Label>()(label);
  if (-index >= index)
    return format("{}", (index + 1));
  else
    return format("{}*", -index);
}

template <typename Surface>
intervalxt::Label Lengths<Surface>::toLabel(const Edge e) const {
  LIBFLATSURF_ASSERT(lengths[e], "Cannot interact with edge that is vertical or not part of the original contour but " << e << " is of this type in " << *this);
  return Label(e.index());
}

template <typename Surface>
Edge Lengths<Surface>::fromLabel(Label l) const {
  const int index = static_cast<int>(std::hash<Label>()(l));
  Edge e(index + 1);
  LIBFLATSURF_ASSERT(toLabel(e) == l, "fromLabel is not the inverse of toLabel");
  LIBFLATSURF_ASSERT(lengths[e], "Cannot interact with vertical edge " << e << " in " << *this);
  return e;
}

template <typename Surface>
typename Surface::Coordinate Lengths<Surface>::length() const {
  LIBFLATSURF_ASSERT(sum >= 0, "Length must not be negative");
  return sum;
}

template <typename Surface>
typename Surface::Coordinate Lengths<Surface>::length(intervalxt::Label label) const {
  auto length = vertical->projectPerpendicular(*lengths[fromLabel(label)]);
  LIBFLATSURF_ASSERT(length > 0, "length must be positive");
  return length;
}

template <typename Surface>
::intervalxt::Lengths Lengths<Surface>::forget() const {
  std::vector<T> lengths;
  this->lengths.apply([&](const auto& e, const auto& v) {
    if (v) {
      lengths.push_back(length(toLabel(e)));
    } else {
      lengths.emplace_back();
    }
  });

  return ::intervalxt::sample::Lengths<T>(lengths);
}

template <typename Surface>
::intervalxt::Lengths Lengths<Surface>::only(const std::unordered_set<Label>& labels) const {
  return forget().only(labels);
}

template <typename Surface>
bool Lengths<Surface>::similar(::intervalxt::Label a, ::intervalxt::Label b, const ::intervalxt::Lengths& other, ::intervalxt::Label aa, ::intervalxt::Label bb) const {
  return forget().similar(a, b, other, aa, bb);
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const Lengths<Surface>& self) {
  using std::string;
  std::vector<string> items;
  self.lengths.apply([&](const auto& key, const auto& value) {
    if (!value) return;
    items.push_back(fmt::format("{}: {} = {}", key, *value, self.length(self.toLabel(key))));
  });
  return os << fmt::format("{}", fmt::join(items, ", "));
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), Lengths, LIBFLATSURF_SURFACE_TYPES)
