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

#ifndef LIBFLATSURF_CHAIN_HPP
#define LIBFLATSURF_CHAIN_HPP

#include <iosfwd>
#include <memory>

#include <boost/operators.hpp>
#include <gmpxx.h>

#include "serializable.hpp"
#include "copyable.hpp"

namespace flatsurf {

template <typename Surface>
class Chain :
  public Serializable<Chain<Surface>>,
  boost::equality_comparable<Chain<Surface>>,
  boost::less_than_comparable<Chain<Surface>, Bound>,
  boost::additive<Chain<Surface>, HalfEdge>,
  boost::additive<Chain<Surface>>,
  boost::multipliable<Chain<Surface>, mpz_class> {

  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");
  
public:
  Chain();
  explicit Chain(std::shared_ptr<const Surface>);

  operator typename Surface::Vector() const;

  const Surface& surface() const;

  Chain<Surface>& operator+=(const Chain&);
  Chain<Surface>& operator-=(const Chain&);
  Chain<Surface>& operator+=(HalfEdge);
  Chain<Surface>& operator-=(HalfEdge);

  Chain<Surface>& operator*=(const mpz_class&);

  mpz_class operator[](const Edge&) const;

  bool operator==(const Chain& rhs) const;

  bool operator>(const Bound) const;
  bool operator<(const Bound) const;

  Chain<Surface> operator-() const;

  explicit operator bool() const;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const Chain<S>&);

 private:
  using Implementation = ::flatsurf::Implementation<Chain>;
  Copyable<Implementation> impl;
  friend Implementation;
  friend std::hash<Chain<Surface>>;
};

template <typename Surface>
Chain(std::shared_ptr<Surface>) -> Chain<Surface>;

}

namespace std {

template <typename Surface>
struct hash<::flatsurf::Chain<Surface>> { size_t operator()(const ::flatsurf::Chain<Surface>&) const noexcept; };

}

#endif
