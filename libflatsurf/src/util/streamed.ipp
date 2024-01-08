/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2022 Julian Rüth
 *
 *  flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  flatsurf is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with flatsurf. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#ifndef LIBFLATSURF_UTIL_STREAMED_IPP
#define LIBFLATSURF_UTIL_STREAMED_IPP

#include <fmt/ostream.h>

namespace {

// Since fmt 9, the ostream operator<< is not used automatically anymore,
// even if fmt/ostream.h has been included.
template <typename T>
auto streamed(const T& x) {
#if FMT_VERSION >= 90000
  return fmt::streamed(x);
#else
  return x;
#endif
}

}

#endif
