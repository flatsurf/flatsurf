/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020 Julian Rüth
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

#ifndef LIBFLATSURF_IMPL_ASSERT_CONNECTION_HPP
#define LIBFLATSURF_IMPL_ASSERT_CONNECTION_HPP

#include <gmpxx.h>

#include "../../flatsurf/forward.hpp"

namespace flatsurf {

template <typename T>
class AssertConnection {
 public:
  bool operator()(const SaddleConnection<FlatTriangulation<T>>&);

 private:
  int budget = 1024;
  long long cost = 1;
};

}  // namespace flatsurf

#endif
