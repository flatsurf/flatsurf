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

// Include this header to get {fmt} support for all types define by this library.

// Note that we would have liked to give {fmt} first class support and deduce
// ostream<< from this. However, this is not possible with the encapsulation
// that we are presently using: Since a Formatter's format() is templatized, it
// cannot be in a .cc file. Of course printing should not use any private
// information that is not readily available otherwise, but we do not want
// implementations in header files and also, this would mean that every header
// has to pull in <fmt/core.h> or we'd need some other header scheme for this.

#ifndef LIBFLATSURF_FMT_HPP
#define LIBFLATSURF_FMT_HPP

#include <fmt/core.h>

#include <sstream>

#include "forward.hpp"

namespace flatsurf {

template <typename T>
struct GenericFormatter {
  constexpr auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const T& value, FormatContext& ctx) {
    std::ostringstream ss;
    ss << value;
    return fmt::format_to(ctx.out(), "{}", ss.str());
  }
};

}

// TODO: Add everything here.
template <typename T>
struct fmt::formatter<::flatsurf::SaddleConnection<T>> : ::flatsurf::GenericFormatter<::flatsurf::SaddleConnection<T>> {};
template <typename T>
struct fmt::formatter<::flatsurf::FlowConnection<T>> : ::flatsurf::GenericFormatter<::flatsurf::FlowConnection<T>> {};
template <>
struct fmt::formatter<::flatsurf::HalfEdge> : ::flatsurf::GenericFormatter<::flatsurf::HalfEdge> {};
template <typename T>
struct fmt::formatter<::flatsurf::Vector<T>> : ::flatsurf::GenericFormatter<::flatsurf::Vector<T>> {};
template <typename T>
struct fmt::formatter<::flatsurf::ContourComponent<T>> : ::flatsurf::GenericFormatter<::flatsurf::ContourComponent<T>> {};
template <>
struct fmt::formatter<::flatsurf::HalfEdgeSet> : ::flatsurf::GenericFormatter<::flatsurf::HalfEdgeSet> {};
template <typename T>
struct fmt::formatter<::flatsurf::FlatTriangulation<T>> : ::flatsurf::GenericFormatter<::flatsurf::FlatTriangulation<T>> {};
template <typename T>
struct fmt::formatter<::flatsurf::FlatTriangulationCollapsed<T>> : ::flatsurf::GenericFormatter<::flatsurf::FlatTriangulationCollapsed<T>> {};

#endif
