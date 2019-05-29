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

#ifndef LIBFLATSURF_UTIL_FALSE_IPP
#define LIBFLATSURF_UTIL_FALSE_IPP

namespace flatsurf {
// A templatized constant that is false. This helps to use static_assert in
// branches of constexpr-if. Just using a plain false leads to compile errors
// so we must make sure the compiler tries not to instantiate this if it's not
// on an active branch.
template <auto = 0>
constexpr bool false_value_v = false;
template <typename = void>
constexpr bool false_type_v = false;
}  // namespace flatsurf

#endif
