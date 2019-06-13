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

#include <exact-real/arb.hpp>
#include <optional>

#include "flatsurf/length_along_triangulation.hpp"
#include "flatsurf/vector_along_triangulation.hpp"
#include "flatsurf/vector.hpp"
#include "flatsurf/half_edge.hpp"

// TODO: Many of the assertions here should be argument checks.
// TODO: This should not be backed by a VectorAlongTriangulation. We should
// just keep the HalfEdgeMap and each "vector" of the squashed triangulation
// should just be the scalar product without multiplying by the horizontal
// vector. (To do this, it's probably easiest to change projection to return a
// triangulation with only the x component. In turn, projection on a vector
// should just return a T and not an actual vector anymore.)

using std::optional;

namespace flatsurf {
namespace {
enum CLASSIFICATION {
  BOTH_ARE_ZERO,
  LEFT_IS_ZERO,
  RIGHT_IS_ZERO,
  NONE_IS_ZERO,
};
}

template <typename T>
class LengthAlongTriangulation<T>::Implementation {
 public:
  // TODO: Pick an approximation automatically (or make it selectable as a
  // template parameter.)
  using Vector = VectorAlongTriangulation<T>;

  Implementation() : value() {}

  Implementation(const Surface& parent, const HalfEdge e) : value(parent) {
    *value += e;
  }

  CLASSIFICATION classify(const Implementation& rhs) const {
    bool is_zero = !value || !*value;
    bool rhs_is_zero = !rhs.value || !*rhs.value;
    if (is_zero && rhs_is_zero) {
      return BOTH_ARE_ZERO;
    }
    if (is_zero) {
      return LEFT_IS_ZERO;
    }
    if (rhs_is_zero) {
      return RIGHT_IS_ZERO;
    }

    assert(value->ccw(*rhs.value) == CCW::COLLINEAR && "Can only compare lengths that come from collinear vectors.");
    assert(value->orientation(*rhs.value) == ORIENTATION::SAME && "Lengths must be non-negative.");

    return NONE_IS_ZERO;
  }

  optional<Vector> value;
};

template <typename T>
LengthAlongTriangulation<T>::LengthAlongTriangulation() : impl(spimpl::make_impl<Implementation>()) {}

template <typename T>
LengthAlongTriangulation<T>::LengthAlongTriangulation(const Surface& parent, const HalfEdge e) : impl(spimpl::make_impl<Implementation>(parent, e)) {}

template <typename T>
bool LengthAlongTriangulation<T>::operator==(const LengthAlongTriangulation& rhs) const {
  switch(impl->classify(*rhs.impl)) {
    case BOTH_ARE_ZERO:
      return true;
    case LEFT_IS_ZERO:
    case RIGHT_IS_ZERO:
      return false;
    case NONE_IS_ZERO:
      return *impl->value == *rhs.impl->value;
  }
}

template <typename T>
bool LengthAlongTriangulation<T>::operator<(const LengthAlongTriangulation& rhs) const {
  switch(impl->classify(*rhs.impl)) {
    case BOTH_ARE_ZERO:
      return false;
    case LEFT_IS_ZERO:
      return true;
    case RIGHT_IS_ZERO:
      return false;
    case NONE_IS_ZERO:
      switch(impl->value->orientation(*impl->value - *rhs.impl->value)) {
        case ORIENTATION::ORTHOGONAL:
          assert(*this == rhs);
          return false;
        case ORIENTATION::SAME:
          return false;
        case ORIENTATION::OPPOSITE:
          return true;
      }
  }
}

template <typename T>
LengthAlongTriangulation<T>& LengthAlongTriangulation<T>::operator+=(const LengthAlongTriangulation& rhs) {
  switch(impl->classify(*rhs.impl)) {
    case BOTH_ARE_ZERO:
    case RIGHT_IS_ZERO:
      return *this;
    case LEFT_IS_ZERO:
      impl->value = *rhs.impl->value;
      return *this;
    case NONE_IS_ZERO:
      *impl->value += *rhs.impl->value;
      return *this;
  }
}

template <typename T>
LengthAlongTriangulation<T>& LengthAlongTriangulation<T>::operator-=(const LengthAlongTriangulation& rhs) {
  switch(impl->classify(*rhs.impl)) {
    case BOTH_ARE_ZERO:
    case RIGHT_IS_ZERO:
      return *this;
    case LEFT_IS_ZERO:
      throw std::logic_error("Can not subtract non-zero length from zero length.");
      return *this;
    case NONE_IS_ZERO:
      assert(rhs <= *this && "Can not subtract length from smaller length.");
      *impl->value -= *rhs.impl->value;
      assert(impl->value->orientation(*rhs.impl->value) != ORIENTATION::OPPOSITE && "Lengths must not be negative");
      return *this;
  }
}

template <typename T>
LengthAlongTriangulation<T>& LengthAlongTriangulation<T>::operator*=(const Quotient& rhs) {
  if (impl->value) {
    *impl->value *= rhs;
  }
  return *this;
}

template <typename T>
LengthAlongTriangulation<T>::operator bool() const noexcept {
  return impl->value && *impl->value;
}

template <typename T>
typename LengthAlongTriangulation<T>::Quotient LengthAlongTriangulation<T>::operator/(const LengthAlongTriangulation& rhs) {
  assert(rhs && "Division by zero length");
  if (!*this) {
    return mpz_class(0);
  }

  // TODO: Implement me
  throw std::logic_error("not implemented: operator/");
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const LengthAlongTriangulation<T>& self) {
  if (!self)
    return os << 0;
  else
    return os << "LengthOf(" << *self.impl->value << ")";
}
}

// Instantiations of templates so implementations are generated for the linker
#include <e-antic/renfxx.h>
#include <exact-real/element.hpp>
#include <exact-real/integer_ring_traits.hpp>
#include <exact-real/number_field_traits.hpp>
#include <exact-real/rational_field_traits.hpp>

namespace flatsurf {
using eantic::renf_elem_class;
using exactreal::Element;
using exactreal::IntegerRingTraits;
using exactreal::NumberFieldTraits;
using exactreal::RationalFieldTraits;

template class LengthAlongTriangulation<long long>;
template std::ostream& operator<<(std::ostream&, const LengthAlongTriangulation<long long>&);
template class LengthAlongTriangulation<renf_elem_class>;
template std::ostream& operator<<(std::ostream&, const LengthAlongTriangulation<renf_elem_class>&);
template class LengthAlongTriangulation<Element<IntegerRingTraits>>;
template std::ostream& operator<<(std::ostream&, const LengthAlongTriangulation<Element<IntegerRingTraits>>&);
template class LengthAlongTriangulation<Element<RationalFieldTraits>>;
template std::ostream& operator<<(std::ostream&, const LengthAlongTriangulation<Element<RationalFieldTraits>>&);
template class LengthAlongTriangulation<Element<NumberFieldTraits>>;
template std::ostream& operator<<(std::ostream&, const LengthAlongTriangulation<Element<NumberFieldTraits>>&);
}
