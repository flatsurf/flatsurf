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

}  // namespace flatsurf

template <typename T>
struct fmt::formatter<::flatsurf::Vector<T>> : ::flatsurf::GenericFormatter<::flatsurf::Vector<T>> {};
template <>
struct fmt::formatter<::flatsurf::FlatTriangulationCombinatorial> : ::flatsurf::GenericFormatter<::flatsurf::FlatTriangulationCombinatorial> {};
template <typename T>
struct fmt::formatter<::flatsurf::FlatTriangulation<T>> : ::flatsurf::GenericFormatter<::flatsurf::FlatTriangulation<T>> {};
template <typename T>
struct fmt::formatter<::flatsurf::FlatTriangulationCollapsed<T>> : ::flatsurf::GenericFormatter<::flatsurf::FlatTriangulationCollapsed<T>> {};
template <typename Surface>
struct fmt::formatter<::flatsurf::Vertical<Surface>> : ::flatsurf::GenericFormatter<::flatsurf::Vertical<Surface>> {};
template <typename Surface>
struct fmt::formatter<::flatsurf::Chain<Surface>> : ::flatsurf::GenericFormatter<::flatsurf::Chain<Surface>> {};
template <typename Surface>
struct fmt::formatter<::flatsurf::SaddleConnections<Surface>> : ::flatsurf::GenericFormatter<::flatsurf::SaddleConnections<Surface>> {};
template <typename Surface>
struct fmt::formatter<::flatsurf::ContourConnection<Surface>> : ::flatsurf::GenericFormatter<::flatsurf::ContourConnection<Surface>> {};
template <typename Surface>
struct fmt::formatter<::flatsurf::SaddleConnection<Surface>> : ::flatsurf::GenericFormatter<::flatsurf::SaddleConnection<Surface>> {};
template <>
struct fmt::formatter<::flatsurf::HalfEdge> : ::flatsurf::GenericFormatter<::flatsurf::HalfEdge> {};
template <>
struct fmt::formatter<::flatsurf::Edge> : ::flatsurf::GenericFormatter<::flatsurf::Edge> {};
template <>
struct fmt::formatter<::flatsurf::Vertex> : ::flatsurf::GenericFormatter<::flatsurf::Vertex> {};
template <typename T>
struct fmt::formatter<::flatsurf::Tracking<T>> : ::flatsurf::GenericFormatter<::flatsurf::Tracking<T>> {};
template <typename K, typename V>
struct fmt::formatter<::flatsurf::TrackingMap<K, V>> : ::flatsurf::GenericFormatter<::flatsurf::TrackingMap<K, V>> {};
template <>
struct fmt::formatter<::flatsurf::HalfEdgeSet> : ::flatsurf::GenericFormatter<::flatsurf::HalfEdgeSet> {};
template <>
struct fmt::formatter<::flatsurf::HalfEdgeSetIterator> : ::flatsurf::GenericFormatter<::flatsurf::HalfEdgeSetIterator> {};
template <typename T>
struct fmt::formatter<::flatsurf::Permutation<T>> : ::flatsurf::GenericFormatter<::flatsurf::Permutation<T>> {};
template <>
struct fmt::formatter<::flatsurf::Bound> : ::flatsurf::GenericFormatter<::flatsurf::Bound> {};
template <typename Surface>
struct fmt::formatter<::flatsurf::IntervalExchangeTransformation<Surface>> : ::flatsurf::GenericFormatter<::flatsurf::IntervalExchangeTransformation<Surface>> {};
template <typename Surface>
struct fmt::formatter<::flatsurf::ContourDecomposition<Surface>> : ::flatsurf::GenericFormatter<::flatsurf::ContourDecomposition<Surface>> {};
template <typename Surface>
struct fmt::formatter<::flatsurf::ContourComponent<Surface>> : ::flatsurf::GenericFormatter<::flatsurf::ContourComponent<Surface>> {};
template <typename Surface>
struct fmt::formatter<::flatsurf::FlowDecomposition<Surface>> : ::flatsurf::GenericFormatter<::flatsurf::FlowDecomposition<Surface>> {};
template <typename Surface>
struct fmt::formatter<::flatsurf::FlowComponent<Surface>> : ::flatsurf::GenericFormatter<::flatsurf::FlowComponent<Surface>> {};
template <typename Surface>
struct fmt::formatter<::flatsurf::DecompositionStep<Surface>> : ::flatsurf::GenericFormatter<::flatsurf::DecompositionStep<Surface>> {};
template <typename Surface>
struct fmt::formatter<::flatsurf::FlowConnection<Surface>> : ::flatsurf::GenericFormatter<::flatsurf::FlowConnection<Surface>> {};
template <typename Surface>
struct fmt::formatter<::flatsurf::MaybeVerticalFlowConnection<Surface>> : ::flatsurf::GenericFormatter<::flatsurf::MaybeVerticalFlowConnection<Surface>> {};
template <typename Surface>
struct fmt::formatter<::flatsurf::Path<Surface>> : ::flatsurf::GenericFormatter<::flatsurf::Path<Surface>> {};
template <typename Surface>
struct fmt::formatter<::flatsurf::PathIterator<Surface>> : ::flatsurf::GenericFormatter<::flatsurf::PathIterator<Surface>> {};

#endif
