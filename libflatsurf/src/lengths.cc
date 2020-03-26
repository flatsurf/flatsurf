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

#include <ostream>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <intervalxt/interval_exchange_transformation.hpp>
#include <intervalxt/length.hpp>
#include <intervalxt/sample/arithmetic.hpp>
#include <intervalxt/sample/e-antic-arithmetic.hpp>
#include <intervalxt/sample/exact-real-arithmetic.hpp>
#include <intervalxt/sample/lengths.hpp>
#include <intervalxt/sample/rational-arithmetic.hpp>

#include "../flatsurf/chain.hpp"
#include "../flatsurf/interval_exchange_transformation.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/vertical.hpp"

#include "external/gmpxxll/gmpxxll/mpz_class.hpp"
#include "external/rx-ranges/include/rx/ranges.hpp"

#include "impl/flow_component.impl.hpp"
#include "impl/flow_connection.impl.hpp"
#include "impl/lengths.hpp"
#include "impl/saddle_connection.impl.hpp"

#include "util/assert.ipp"
#include "util/false.ipp"

namespace flatsurf {

using fmt::format;
using intervalxt::Label;
using intervalxt::Length;
using rx::none_of;

template <typename Surface>
Lengths<Surface>::Lengths(std::shared_ptr<const Vertical<FlatTriangulation<T>>> vertical, EdgeMap<std::optional<SaddleConnection<FlatTriangulation<T>>>>&& lengths) :
  vertical(vertical),
  lengths(std::move(lengths)),
  stack(),
  sum(),
  degree(0) {
  this->lengths.apply([&](const auto& edge, const auto& connection) {
    CHECK_ARGUMENT(!connection || vertical->perpendicular(*connection) > 0, "nontrivial length must be positive but " << edge << " is " << *connection);
    if (connection)
      degree = std::max(degree, coefficients(toLabel(edge)).size());
  });
}

template <typename Surface>
void Lengths<Surface>::push(Label label) {
  ASSERT(stack | none_of([&](const auto& l) { return l == label; }), "must not push the same label twice");
  stack.push_back(label);
  sum += length(label);
}

template <typename Surface>
void Lengths<Surface>::pop() {
  ASSERT(not stack.empty(), "cannot pop from an empty stack");
  sum -= length(stack.back());
  stack.pop_back();
  ASSERT(!stack.empty() || sum == T(), "sum inconsistent with stack");
}

template <typename Surface>
void Lengths<Surface>::subtract(Label minuend) {
  subtractRepeated(minuend, 1);
}

template <typename Surface>
void Lengths<Surface>::subtractRepeated(Label minuend, const mpz_class& iterations) {
  ASSERT(iterations > 0, "must subtract at least once");
  ASSERT(length(minuend) > 0, "lengths must be positive");

  const T expected = [&]() {
    if constexpr (std::is_same_v<T, long long>)
      return length(minuend) - iterations.get_ui() * length();
    else
      return length(minuend) - iterations * length();
  }();
  ASSERT(expected > 0, "Lengths must be positive but subtracting " << length() << " " << iterations << "times from edge " << fromLabel(minuend) << " of length " << length(minuend) << " would yield " << expected << " which is non-positive.");
  ASSERT(expected < length(minuend), "subtraction must shorten lengths");

  auto& component = *std::find_if(begin(state.lock()->components), end(state.lock()->components), [&](const auto& component) {
    return static_cast<::intervalxt::Label>(*begin(component.dynamicalComponent.topContour())) == minuend || static_cast<::intervalxt::Label>(*begin(component.dynamicalComponent.bottomContour())) == minuend;
  });

  bool minuendOnTop = static_cast<::intervalxt::Label>(*begin(component.dynamicalComponent.topContour())) == minuend;
  ASSERT(minuendOnTop || static_cast<::intervalxt::Label>(*begin(component.dynamicalComponent.bottomContour())) == minuend, "minuend not found on any contour");

  const auto minuendContour = minuendOnTop ? component.dynamicalComponent.topContour() : component.dynamicalComponent.bottomContour();
  const auto subtrahendContour = minuendOnTop ? component.dynamicalComponent.bottomContour() : component.dynamicalComponent.topContour();

  const auto flow = [&](const auto& connections, bool reverse) {
    auto flowed = connections | rx::transform([&](const auto& connection) {
      return ImplementationOf<FlowConnection<FlatTriangulation<T>>>::make(state.lock(), ImplementationOf<FlowComponent<FlatTriangulation<T>>>::make(state.lock(), &component), connection).saddleConnection();
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
    Chain walk(minuendConnection->surface().shared_from_this());

    // Walk down on the minuend's left boundary
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
        ASSERT(subtrahend != *rbegin(subtrahendContour), "Stack cannot contain every label in the IntervalExchangeTransformation when subtracting.");

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
    // connections on its left.  Therefore we need to caller to do a simple
    // subtract before doing a subtractRepeated.
    walk *= iterations;

    auto vector = walk + *minuendConnection;

    // The tricky part is now to figure out the actual source/target sector of
    // the updated SaddleConnection.
    // This should probably be moved into SaddleConnection at some point.

    ASSERT(vertical->perpendicular(vector) < 0, "Length must be positive.");

    // We know that minuendConnection (which we reversed in the beginning) is
    // pointing left and that the new -minuendConnection must point left into
    // the same half plane.
    auto source = minuendConnection->source();
    while (vertical->perpendicular(minuendConnection->surface().fromEdge(source)) < 0)
      source = minuendConnection->surface().previousAtVertex(source);
    while (!minuendConnection->surface().inSector(source, vector))
      source = minuendConnection->surface().nextAtVertex(source);

    while (vertical->perpendicular(minuendConnection->surface().fromEdge(target)) <= 0)
      target = minuendConnection->surface().previousAtVertex(target);
    while (!minuendConnection->surface().inSector(target, -vector))
      target = minuendConnection->surface().nextAtVertex(target);

    minuendConnection = SaddleConnection<FlatTriangulation<T>>(minuendConnection->surface().shared_from_this(), target, source, -vector);

    ASSERT(minuendConnection->source() == target && minuendConnection->target() == source, "We tried to get SaddleConnection source/target right but SaddleConnection consturctor disagrees.");

    ASSERTIONS([&]() {
      // A very similar bit of code lives in FlatTriangulationCollapsed. We
      // might want to consolidate these when we touch this code again.
      static Amortized cost;

      const auto abs = [](const auto& x) {
        return x < 0 ? -x : x;
      };

      const auto relativeCost = [&](const Vector<T>& divident, const Vector<T>& divisor) -> mpz_class {
        return gmpxxll::mpz_class(::intervalxt::sample::Arithmetic<T>::floorDivision(divident * divident, abs(divident * divisor)));
      };

      if (!cost.pay(relativeCost(static_cast<const Vector<T>&>(*minuendConnection), minuendConnection->surface().shortest(*minuendConnection)) + 1)) return;

      const auto reconstruction = SaddleConnection<FlatTriangulation<T>>::inSector(minuendConnection->surface().shared_from_this(), minuendConnection->source(), *minuendConnection);
      ASSERT(*minuendConnection == reconstruction,
          "Connection after subtract does not actually exist in surface. We claimed it's " << *minuendConnection << " but it is more likely " << reconstruction);
    });
  }

  ASSERT(get(minuend), "lengths must be non-zero");
  ASSERT(length(minuend) == expected, "subtract inconsistent: subtracted " << length() << " from " << fromLabel(minuend) << " which should have yielded " << expected << " but got " << length(minuend) << " instead");

  stack.clear();
  sum = T();
}

template <typename Surface>
void Lengths<Surface>::registerDecomposition(std::shared_ptr<FlowDecompositionState<FlatTriangulation<T>>> state) {
  this->state = state;
}

template <typename Surface>
Label Lengths<Surface>::subtractRepeated(Label minuend) {
  // This should be optimized, see https://github.com/flatsurf/flatsurf/issues/155.
  auto ret = stack.back();
  subtract(minuend);
  return ret;
}

template <typename Surface>
std::vector<mpq_class> Lengths<Surface>::coefficients(Label label) const {
  auto coefficients = intervalxt::sample::Arithmetic<T>::coefficients(length(label));
  if (coefficients.size() == 0 || coefficients.size() == 1) {
    while (coefficients.size() < degree)
      coefficients.emplace_back();
  }
  return coefficients;
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
  ASSERT(lengths[e], "Cannot interact with edge that is vertical or not part of the original contour but " << e << " is of this type in " << *this);
  return Label(e.index());
}

template <typename Surface>
Edge Lengths<Surface>::fromLabel(Label l) const {
  const int index = static_cast<int>(std::hash<Label>()(l));
  Edge e(index + 1);
  ASSERT(toLabel(e) == l, "fromLabel is not the inverse of toLabel");
  ASSERT(lengths[e], "Cannot interact with vertical edge " << e << " in " << *this);
  return e;
}

template <typename Surface>
typename Surface::Coordinate Lengths<Surface>::length() const {
  ASSERT(sum >= 0, "Length must not be negative");
  return sum;
}

template <typename Surface>
typename Surface::Coordinate Lengths<Surface>::length(intervalxt::Label label) const {
  auto length = vertical->perpendicular(*lengths[fromLabel(label)]);
  ASSERT(length > 0, "length must be positive");
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
