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

#ifndef LIBFLATSURF_INTERVAL_EXCHANGE_TRANSFORMATION_HPP
#define LIBFLATSURF_INTERVAL_EXCHANGE_TRANSFORMATION_HPP

#include <intervalxt/maybe_saddle_connection.hpp>
#include "external/spimpl/spimpl.h"

#include "flatsurf/forward.hpp"

namespace flatsurf {
template <class T>
class IntervalExchangeTransformation {
 public:
  IntervalExchangeTransformation(const FlatTriangulation<T> &, const Vector<T> &vertical);

  template <typename S>
  friend std::ostream &operator<<(std::ostream &, const IntervalExchangeTransformation<S> &);

 private:
  class Implementation;
  spimpl::unique_impl_ptr<Implementation> impl;
};
}  // namespace flatsurf

#endif
