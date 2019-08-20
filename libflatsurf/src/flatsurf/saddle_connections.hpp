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

#ifndef LIBFLATSURF_SADDLE_CONNECTIONS_HPP
#define LIBFLATSURF_SADDLE_CONNECTIONS_HPP

#include <boost/iterator/iterator_facade.hpp>
#include <optional>
#include "external/spimpl/spimpl.h"

#include "flatsurf/forward.hpp"
#include "flatsurf/half_edge.hpp"
#include "flatsurf/vertex.hpp"

namespace flatsurf {
template <typename Surface>
class SaddleConnections {
 public:
  // All saddle connections on the surface starting at any vertex.
  SaddleConnections(const std::shared_ptr<const Surface> &, Bound searchRadius);

  // All saddle connections on the surface starting at source.
  SaddleConnections(const std::shared_ptr<const Surface> &, Bound searchRadius, const Vertex source);

  // The saddle connections that are starting at the source of sectorBegin
  // and lie in the sector between sectorBegin and the follow half edge in
  // counter-clockwise order.
  SaddleConnections(const std::shared_ptr<const Surface> &, Bound searchRadius, HalfEdge sectorBegin);

  class Iterator : public boost::iterator_facade<Iterator, const std::unique_ptr<SaddleConnection<Surface>>, std::forward_iterator_tag, const std::unique_ptr<SaddleConnection<Surface>>> {
    class Implementation;
    spimpl::impl_ptr<Implementation> impl;

    friend SaddleConnections;

   public:
    Iterator(spimpl::impl_ptr<Implementation> &&impl);

    // Advance the iterator to the next saddle connection.
    void increment();
    // Advance the iterator to the next saddle connection or until a HalfEdge is
    // being crossed during the search (in forward direction.) This can be useful
    // if information about the exact path in the surface for a saddle connection
    // needs to be reconstructed.
    std::optional<HalfEdge> incrementWithCrossings();
    bool equal(const Iterator &other) const;
    std::unique_ptr<SaddleConnection<Surface>> dereference() const;

    void skipSector(CCW sector);

// Detect GCC (and skip clang/cling so we do not see warnings): https://stackoverflow.com/questions/38499462/how-to-tell-clang-to-stop-pretending-to-be-other-compilers
#if defined(__GNUC__) && !defined(__llvm__)
#pragma GCC diagnostic push
// We cannot make the following operator a template: https://stackoverflow.com/questions/18823618/overload-operator-for-nested-class-template
#pragma GCC diagnostic ignored "-Wnon-template-friend"
#endif
    friend std::ostream &operator<<(std::ostream &, const Iterator &);
#if defined(__GNUC__) && !defined(__llvm__)
#pragma GCC diagnostic pop
#endif
  };

  Iterator begin() const;
  Iterator end() const;

  template <typename Surf>
  friend std::ostream &operator<<(std::ostream &, const SaddleConnections<Surf> &);

 private:
  class Implementation;
  spimpl::impl_ptr<Implementation> impl;
};

template <typename Surface>
SaddleConnections(const std::shared_ptr<Surface> &, Bound)->SaddleConnections<Surface>;

template <typename Surface>
SaddleConnections(const std::shared_ptr<Surface> &, Bound, const Vertex &)->SaddleConnections<Surface>;

template <typename Surface>
SaddleConnections(const std::shared_ptr<Surface> &, Bound, const HalfEdge &)->SaddleConnections<Surface>;

}  // namespace flatsurf

#endif
