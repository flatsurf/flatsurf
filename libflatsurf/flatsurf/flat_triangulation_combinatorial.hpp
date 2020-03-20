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

#ifndef LIBFLATSURF_FLAT_TRIANGULATION_COMBINATORIAL_HPP
#define LIBFLATSURF_FLAT_TRIANGULATION_COMBINATORIAL_HPP

#include <boost/operators.hpp>
#include <iosfwd>
#include <memory>
#include <vector>

#include "moveable.hpp"
#include "serializable.hpp"

namespace flatsurf {

class FlatTriangulationCombinatorial : Serializable<FlatTriangulationCombinatorial>,
                                       boost::equality_comparable<FlatTriangulationCombinatorial>,
                                       public std::enable_shared_from_this<FlatTriangulationCombinatorial> {
 public:
  FlatTriangulationCombinatorial();
  FlatTriangulationCombinatorial(const std::vector<std::vector<int>> &vertices, const std::vector<int> &boundaries = std::vector<int>());
  FlatTriangulationCombinatorial(const Permutation<HalfEdge> &vertices);
  FlatTriangulationCombinatorial(FlatTriangulationCombinatorial &&);

  // Create an independent clone of this triangulation that is built from the
  // same data. There is no copy-constructor since it is too likely that it
  // would not update the associated HalfEdgeMaps in the way that the caller
  // expects.
  std::unique_ptr<FlatTriangulationCombinatorial> clone() const;

  // Create an independent clone of this triangulation with an extra vertex
  // inserted next to the given half edge e. The numbering of half edges is
  // consistent with the numbering in this triangulation, i.e., to single out
  // the new half edges, you may take a, the half edge that is nextAtVertex to
  // e, and then look at the successors of -a.
  std::unique_ptr<FlatTriangulationCombinatorial> insertAt(HalfEdge e) const;

  // Create an independent clone of this triangulation with an edded boundary
  // at the half edge e by removing the identification of the two corresponding
  // half edges there.
  std::unique_ptr<FlatTriangulationCombinatorial> slot(HalfEdge e) const;

  HalfEdge nextAtVertex(HalfEdge e) const;
  HalfEdge previousAtVertex(HalfEdge e) const;
  HalfEdge nextInFace(HalfEdge e) const;
  HalfEdge previousInFace(HalfEdge e) const;

  // Eliminate e from this triangulation by identifying the two edges of each
  // of its faces, respectively.
  // Return a pair consisting of the half edges describing the collapsed faces;
  // first, the representative of previousAtVertex(e), second the representative
  // of nextAtVertex(e). Or, in other words, this returns the two surviving
  // half edges going out of the collapsed e.
  // Note that due to renumbering of half edges, this might invalidate half
  // edges. Also, this might break some dependent structures such as
  // HalfEdgeMaps or FlatTriangulation surfaces if the identification cannot be
  // done in a way consistent with the stored data.
  virtual std::pair<HalfEdge, HalfEdge> collapse(HalfEdge e);

  // Return whether this surface has a boundary edge.
  bool hasBoundary() const;
  // Return whether this is a boundary half edge, i.e., there is no successor
  // in counter-clockwise order of this half edge at its source.
  bool boundary(HalfEdge e) const;

  const std::vector<Edge> &edges() const;
  const std::vector<HalfEdge> &halfEdges() const;
  const std::vector<Vertex> &vertices() const;

  // Return the outgoing half edges from this vertex.
  std::vector<HalfEdge> outgoing(const Vertex&) const;

  // Return the number of edges in this triangulation.
  size_t size() const noexcept;

  virtual void flip(HalfEdge);

  // Return the connected components of this flat triangulation (in no specific
  // order.)
  // Connected components are the equivalence classes induced by the smallest
  // equivalence relation which satisfies:
  // * The half edges e and -e are equivalent.
  // * Two half edges in the same face are equivalent.
  // Note that components that only meet in a vertex but not a half edge are therefore distinct.
  std::vector<std::vector<HalfEdge>> components() const;

  // Return whether rhs is combinatorial the same triangulation (with the same
  // numbering of edges.)
  // This method is not virtual so that non-combinatorial triangulations can
  // also be compared combinatorically.
  bool operator==(const FlatTriangulationCombinatorial &rhs) const noexcept;

  FlatTriangulationCombinatorial &operator=(FlatTriangulationCombinatorial &&) noexcept;

  friend std::ostream &operator<<(std::ostream &, const FlatTriangulationCombinatorial &);

 private:
  using Implementation = ImplementationOf<FlatTriangulationCombinatorial>;
  Moveable<Implementation> impl;
  friend Implementation;
};
}  // namespace flatsurf

#endif
