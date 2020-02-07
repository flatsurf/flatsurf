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

#ifndef LIBFLATSURF_COPYABLE_HPP
#define LIBFLATSURF_COPYABLE_HPP

#include "external/spimpl/spimpl.h"

#include "forward.hpp"

namespace flatsurf {

// Note that this cannot be turned into a base class Copyable<T> sanely since
// its impl field would not be visible when T is a template, i.e., we would
// have to use this all over the place in the member implementations.
template <typename T>
using Copyable = spimpl::impl_ptr<T>;

}  // namespace flatsurf

#endif
