/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2022 Julian Rüth
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
#ifndef LIBFLATSURF_VECTOR_HPP
#define LIBFLATSURF_VECTOR_HPP

#include <exact-real/forward.hpp>
#include <iosfwd>

#include "copyable.hpp"
#include "detail/vector_exact.hpp"
#include "detail/vector_with_error.hpp"

namespace flatsurf {

// A vector in ℝ² whose coordinates are of type T.
template <typename T>
class Vector : public std::conditional_t<std::is_same_v<T, exactreal::Arb>, detail::VectorWithError<Vector<T>>, detail::VectorExact<Vector<T>, T>> {
 public:
  using Coordinate = T;

  Vector() noexcept;
  Vector(const Coordinate& x, const Coordinate& y);
  Vector(Coordinate&& x, Coordinate&& y);

  template <typename X, typename Y, std::enable_if_t<!std::is_convertible_v<X, Coordinate> || !std::is_convertible_v<Y, Coordinate>, int> = 0>
  Vector(X&& x, Y&& y) :
    Vector(static_cast<Coordinate>(std::forward<X>(x)), static_cast<Coordinate>(std::forward<Y>(y))) {}

  const Coordinate& x() const;
  const Coordinate& y() const;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const Vector<S>&);

 private:
  friend detail::VectorExact<Vector<T>, T>;
  friend detail::VectorWithError<Vector<T>>;
  friend detail::VectorBase<Vector<T>>;
  friend std::hash<Vector<T>>;

  friend cereal::access;
  template <typename Archive>
  void save(Archive& archive) const;
  template <typename Archive>
  void load(Archive& archive);

  Copyable<Vector<T>> self;
  friend ImplementationOf<Vector<T>>;
};
}  // namespace flatsurf

namespace std {

template <typename T>
struct hash<::flatsurf::Vector<T>> { size_t operator()(const ::flatsurf::Vector<T>&) const; };

}  // namespace std

#endif
