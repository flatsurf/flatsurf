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

#ifndef LIBFLATSURF_PERMUTATION_HPP
#define LIBFLATSURF_PERMUTATION_HPP

#include <functional>
#include <iosfwd>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/operators.hpp>

#include "forward.hpp"

namespace flatsurf {

// A type-safe permutation of Ts.
// There should be no runtime overhead to using a simple T[], at least when
// compiled with -flto.
template <typename T>
class Permutation : boost::equality_comparable<Permutation<T>>, boost::multipliable<Permutation<T>> {
  Permutation(const std::vector<T> &);

 public:
  Permutation();
  explicit Permutation(const std::vector<std::vector<T>> &cycles);
  explicit Permutation(const std::vector<std::pair<T, T>> &permutation);
  explicit Permutation(const std::unordered_map<T, T> &permutation);
  static Permutation<T> random(const std::vector<T> &domain);

  const T &operator()(const T &) const;
  const T &preimage(const T &) const;

  template <typename S>
  friend Permutation<S> &operator*=(const std::vector<S> &cycle, Permutation<S> &);
  template <typename S>
  friend Permutation<S> &operator*=(Permutation<S> &, const std::vector<S> &cycle);

  Permutation<T> &operator*=(const Permutation<T> &);
  Permutation<T> operator~() const;

  template <typename S>
  friend std::ostream &operator<<(std::ostream &, const Permutation<S> &);
  size_t size() const noexcept;
  const std::vector<T> &domain() const noexcept;
  std::vector<std::vector<T>> cycles() const noexcept;
  // Return the cycle containing this T.
  std::vector<T> cycle(const T &) const noexcept;
  void drop(const std::vector<T> &);

  bool trivial() const noexcept;

  bool operator==(const Permutation &) const noexcept;

 private:
  std::vector<T> permutation;
  std::vector<T> inverse;
};
}  // namespace flatsurf

#endif
