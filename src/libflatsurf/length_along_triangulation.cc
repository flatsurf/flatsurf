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

// TODO:
#include <iostream>

#include "flatsurf/length_along_triangulation.hpp"
#include "flatsurf/vector_along_triangulation.hpp"
#include "flatsurf/vector.hpp"
#include "flatsurf/half_edge.hpp"
#include "flatsurf/flat_triangulation.hpp"

// TODO: Many of the assertions here should be argument checks.
// TODO: We do not use any approximate computations here yet, but we should of course.

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
  Implementation() : value() {}

  Implementation(Surface const * parent, Vector<T> const * horizontal, const HalfEdge e) : parent(parent), horizontal(horizontal), value(parent->fromEdge(e) * *horizontal) {
    std::cout<<*parent<<std::endl;
    std::cout<<*horizontal<<std::endl;
    std::cout<<e<<std::endl;
    assert(value >= 0 && "Lenghts must not be negative");
  }

  CLASSIFICATION classify(const Implementation& rhs) const {
    bool is_zero = parent == nullptr || !value;
    bool rhs_is_zero = rhs.parent == nullptr || !rhs.value;
    if (is_zero && rhs_is_zero) {
      return BOTH_ARE_ZERO;
    }
    if (is_zero) {
      return LEFT_IS_ZERO;
    }
    if (rhs_is_zero) {
      return RIGHT_IS_ZERO;
    }

    assert(parent == rhs.parent && "Lengths must be in the same triangulation.");
    assert(horizontal == rhs.horizontal && "Lengths must be relative to the same horizontal vector.");

    return NONE_IS_ZERO;
  }

  Surface const * parent;
  Vector<T> const * horizontal;
  T value;
};

template <typename T>
LengthAlongTriangulation<T>::LengthAlongTriangulation() : impl(spimpl::make_impl<Implementation>()) {}

template <typename T>
LengthAlongTriangulation<T>::LengthAlongTriangulation(Surface const * parent, Vector<T> const * horizontal, const HalfEdge e) : impl(spimpl::make_impl<Implementation>(parent, horizontal, e)) {}

template <typename T>
bool LengthAlongTriangulation<T>::operator==(const LengthAlongTriangulation& rhs) const {
  return impl->value == rhs.impl->value;
}

template <typename T>
bool LengthAlongTriangulation<T>::operator<(const LengthAlongTriangulation& rhs) const {
  return impl->value < rhs.impl->value;
}

template <typename T>
LengthAlongTriangulation<T>& LengthAlongTriangulation<T>::operator+=(const LengthAlongTriangulation& rhs) {
  switch(impl->classify(*rhs.impl)) {
    case BOTH_ARE_ZERO:
    case RIGHT_IS_ZERO:
      return *this;
    case LEFT_IS_ZERO:
      impl = rhs.impl;
      return *this;
    case NONE_IS_ZERO:
      impl->value += rhs.impl->value;
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
      impl->value -= rhs.impl->value;
      assert(impl->value >= 0 && "Lengths must not be negative");
      return *this;
  }
}

template <typename T>
LengthAlongTriangulation<T>& LengthAlongTriangulation<T>::operator*=(const Quotient& rhs) {
  if (impl->value) {
    if constexpr (std::is_same_v<T, long long>) {
      impl->value *= rhs.get_si();
    } else {
      impl->value *= rhs;
    }
  }
  return *this;
}

template <typename T>
LengthAlongTriangulation<T>::operator bool() const noexcept {
  return static_cast<bool>(impl->value);
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
    throw std::logic_error("not implemented: operator<<(LengthAlongTriangulation)");
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
