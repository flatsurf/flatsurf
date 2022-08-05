/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2022 Julian Rüth
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
#ifndef LIBFLATSURF_EQUIVALENCE_HPP
#define LIBFLATSURF_EQUIVALENCE_HPP

#include <type_traits>

#include <tuple>
#include <any>
#include <functional>

#include "copyable.hpp"

namespace flatsurf {

template <typename Surface>
class Equivalence {
  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type parameter must not have modifiers such as const");

 public:
  template <typename T>
  Equivalence(T&& impl) : self(impl) {}

  static Equivalence combinatorial();
  static Equivalence combinatorial(std::function<bool(const Surface&, Edge)>);

  template <typename S>
  friend std::ostream &operator<<(std::ostream &, const Equivalence<S> &);

 private:
  Copyable<Equivalence> self;

  friend ImplementationOf<Equivalence>;
};

template <typename Surface>
struct ImplementationOf<Equivalence<Surface>> {
  virtual ~ImplementationOf();
  virtual std::tuple<std::any, std::size_t> code(const Surface&) = 0;
  virtual Iterable<Deformation<Surface>> automorphisms() = 0;
  virtual void normalize(Surface&) = 0;
  virtual void equal(const ImplementationOf&) = 0;
  virtual std::string toString() = 0;
};

}
#endif
