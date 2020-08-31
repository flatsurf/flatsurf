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

#ifndef LIBFLATSURF_BOUND_HPP
#define LIBFLATSURF_BOUND_HPP

#include <gmpxx.h>

#include <boost/operators.hpp>
#include <iosfwd>

#include "forward.hpp"

namespace flatsurf {

class Bound : boost::equality_comparable<Bound> {
 public:
  Bound();
  Bound(const mpz_class& x, const mpz_class& y);

  template <typename T>
  static Bound lower(const Vector<T>&);

  template <typename T>
  static Bound upper(const Vector<T>&);

  const mpz_class& squared() const noexcept;

  bool operator==(const Bound&) const noexcept;

  // Return the floor division by this bound.
  mpz_class operator/(const Bound&) const;

  friend std::ostream& operator<<(std::ostream&, const Bound&);

 private:
  mpz_class square;

  friend cereal::access;

  template <typename Archive>
  void save(Archive&) const;
  template <typename Archive>
  void load(Archive&);
};

}  // namespace flatsurf

#endif
