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

// TODO
#include <iostream>

#include <ostream>
#include <intervalxt/length.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>

#include <boost/algorithm/string/join.hpp>
#include <boost/lexical_cast.hpp>

#include <intervalxt/sample/arithmetic.hpp>
#include <intervalxt/sample/exact-real-arithmetic.hpp>
#include <intervalxt/sample/e-antic-arithmetic.hpp>
#include <intervalxt/sample/rational-arithmetic.hpp>
#include <intervalxt/interval_exchange_transformation.hpp>

#include "../flatsurf/vertical.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/interval_exchange_transformation.hpp"
#include "../flatsurf/chain.hpp"

#include "external/rx-ranges/include/rx/ranges.hpp"

#include "impl/lengths.hpp"
#include "impl/flow_connection.impl.hpp"
#include "impl/flow_component.impl.hpp"
#include "impl/saddle_connection.impl.hpp"

#include "util/assert.ipp"
#include "util/false.ipp"

namespace flatsurf {

using intervalxt::Label;
using intervalxt::Length;
using rx::none_of;
using fmt::format;

template <typename Surface>
Lengths<Surface>::Lengths(std::shared_ptr<const Vertical<Uncollapsed>> vertical, const EdgeMap<typename Surface::SaddleConnection>& lengths) :
  vertical(vertical),
  lengths(lengths),
  stack(),
  sum(),
  degree(0) {
  lengths.apply([&](const auto& edge, const auto& connection) {
    CHECK_ARGUMENT(!connection || vertical->perpendicular(connection) > 0, "nontrivial length must be positive but " << edge << " is " << connection);
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
  ASSERT(!stack.empty() || sum == typename Surface::Coordinate(), "sum inconsistent with stack");
}

template <typename Surface>
void Lengths<Surface>::subtract(Label minuend) {
  // std::cout<<"Subtracting from " << fromLabel(minuend) << " when " << *this << std::endl;

  ASSERT(length(minuend) > 0, "lengths must be positive");

  const typename Surface::Coordinate expected = length(minuend) - length();
  ASSERT(expected > 0, "Lengths must be positive but subtracting " << length() << " from edge " << fromLabel(minuend) << " of length " << length(minuend) << " would yield " << expected << " which is non-positive.");
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
        return Implementation<FlowConnection<FlatTriangulation<typename Surface::Coordinate>>>::make(state.lock(), ::flatsurf::Implementation<FlowComponent<FlatTriangulation<typename Surface::Coordinate>>>::make(state.lock(), &component), connection).saddleConnection();
        }) | rx::transform([&](const auto& connection) {
          // TODO: This happens to be correct because there can be no HalfEdges
          // when reverse. These would be incorrectly oriented. See comments in
          // intervalxt about orientation of HalfEdge TODO.
          return reverse ? -connection : connection;
          }) | rx::to_vector();
    if (reverse)
      std::reverse(begin(flowed), end(flowed));
    return flowed;
  };

  auto& minuendConnection = lengths.get(fromLabel(minuend));

  minuendConnection = -minuendConnection;

  HalfEdge target = minuendConnection.target();
  Chain walk(minuendConnection.surface().shared_from_this());;
  // Walk down on the minuend's left boundary
  {
    for (const auto& connection : flow(begin(minuendContour)->left(), !minuendOnTop)) {
      walk += connection;
      target = connection.target();
    }
  }

  // Walk across every subtrahend
  {
    for (const auto& subtrahend : subtrahendContour) {
      ASSERT(subtrahend != *rbegin(subtrahendContour), "Stack cannot contain every label in the IntervalExchangeTransformation");
      // std::cout << "subtracting " << subtrahend << std::endl;

      for (const auto& connection : flow(subtrahend.cross(), !minuendOnTop)) {
        // std::cout << "flowing over " << connection << std::endl;
        walk += connection;
        target = connection.target();
      }

      if (static_cast<::intervalxt::Label>(subtrahend) == stack.back())
        break;
    }
  }

  auto source = minuendConnection.source();
  // std::cout << "source= " << source << std::endl;
  if (vertical->perpendicular(minuendConnection.surface().fromEdge(source)) > 0)
    source = minuendConnection.surface().nextAtVertex(source);
  // std::cout << "source= " << source << std::endl;
  ASSERT(vertical->perpendicular(minuendConnection.surface().fromEdge(source)) <= 0, "minuend source edge in wrong half plane");

  auto vector = walk + minuendConnection;

  // TODO: I don't really believe that this is correct.
  // std::cout << "target= " << target << std::endl;
  while (!minuendConnection.surface().inSector(target, -vector))
    target = minuendOnTop ? minuendConnection.surface().previousAtVertex(target) : minuendConnection.surface().nextAtVertex(target);
  // std::cout << "target= " << target << std::endl;

  // TODO: This is ridiculously slow but the source & target in the line above was not correct in general.
  // minuendConnection = SaddleConnection<FlatTriangulation<typename Surface::Coordinate>>::inHalfPlane(minuendConnection.surface().shared_from_this(), source, *vertical, vector);

  // std::cout << "target= " << minuendConnection.target() << std::endl;

  minuendConnection = SaddleConnection(minuendConnection.surface().shared_from_this(), source, target, vector);

  minuendConnection = -minuendConnection;

  // TODO: This is nice but too expensive for large vectors.
  // ::flatsurf::Implementation<SaddleConnection<FlatTriangulation<typename Surface::Coordinate>>>::check(minuendConnection);

  ASSERT(get(minuend), "lengths must be non-zero");
  ASSERT(length(minuend) == expected, "subtract inconsistent: subtracted " << length() << " from " << fromLabel(minuend) << " which should have yielded " << expected << " but got " << length(minuend) << " instead");

  stack.clear();
  sum = typename Surface::Coordinate();

  // std::cout<<"Subtracted from " << fromLabel(minuend) << " when " << *this << std::endl;
}

template <typename Surface>
void Lengths<Surface>::registerDecomposition(std::shared_ptr<FlowDecompositionState<FlatTriangulation<typename Surface::Coordinate>>> state) {
  this->state = state;
}

template <typename Surface>
Label Lengths<Surface>::subtractRepeated(Label minuend) {
  /*
  // TODO: The following is *not* correct. The geometry changes after the first subtraction.
  // TODO: This should be properly accelerated.
  auto stack = this->stack;
  subtract(minuend);
  Label last = stack.back();
  while(true) {
    for (auto l : stack) {
      push(l);
      if (cmp(minuend) >= 0) {
        pop();
        if (length())
          subtract(minuend);
        return last;
      } else {
        last = l;
      }
    }

    // std::cout << "Subtracting repeated… " << ++N << std::endl;
    subtract(minuend);
  }
  */

  auto ret = stack.back();
  subtract(minuend);
  return ret;
}

template <typename Surface>
std::vector<mpq_class> Lengths<Surface>::coefficients(Label label) const {
  auto coefficients = intervalxt::sample::Arithmetic<typename Surface::Coordinate>::coefficients(length(label));
  if (coefficients.size() == 0 || coefficients.size() == 1) {
    // TODO: We are assuming that 0/1 lengths means zero/rational. Is that
    // sane? Probably, yes, but we should assert somewhere else…
    // It's also really weird that we call this from the constructor.
    while(coefficients.size() < degree)
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
intervalxt::Label Lengths<Surface>::toLabel(Edge e) const {
  ASSERT(lengths.get(e), "Cannot interact with edge that is vertical or not part of the original contour but " << e << " is of this type in " << *this);
  return Label(e.index());
}

template <typename Surface>
Edge Lengths<Surface>::fromLabel(Label l) const {
  const int index = static_cast<int>(std::hash<Label>()(l));
  Edge e(index + 1);
  ASSERT(toLabel(e) == l, "fromLabel is not the inverse of toLabel");
  ASSERT(lengths.get(e), "Cannot interact with vertical edge " << e << " in " << *this);
  return e;
}

template <typename Surface>
typename Surface::Coordinate Lengths<Surface>::length() const {
  ASSERT(sum >= 0, "Length must not be negative");
  return sum;
}

template <typename Surface>
typename Surface::Coordinate Lengths<Surface>::length(intervalxt::Label label) const {
  auto length = vertical->perpendicular(lengths.get(fromLabel(label)));
  ASSERT(length > 0, "length must be positive");
  return length;
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const Lengths<Surface>& self) {
  using std::string;
  std::vector<string> items;
  self.lengths.apply([&](const auto& key, const auto& value) {
    string v = boost::lexical_cast<string>(value);
    if (v == "" || v == "0") return;

    string k = boost::lexical_cast<string>(key);
    items.push_back(k + ": " + v + " = " + boost::lexical_cast<string>(self.length(self.toLabel(key))));
  });
  return os << boost::algorithm::join(items, ", ");
}

}

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), Lengths, LIBFLATSURF_SURFACE_TYPES)
