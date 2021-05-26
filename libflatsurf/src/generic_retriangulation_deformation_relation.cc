/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2021 Julian Rüth
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

#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/half_edge_set.hpp"
#include "../flatsurf/half_edge_set_iterator.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/path_iterator.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/orientation.hpp"
#include "../flatsurf/vertex.hpp"
#include "../flatsurf/vertical.hpp"
#include "../flatsurf/ccw.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/saddle_connections_by_length.hpp"
#include "../flatsurf/saddle_connections_by_length_iterator.hpp"
#include "../flatsurf/saddle_connections_iterator.hpp"
#include "impl/generic_retriangulation_deformation_relation.hpp"
#include "util/assert.ipp"

namespace flatsurf {

template <typename Surface>
GenericRetriangulationDeformationRelation<Surface>::GenericRetriangulationDeformationRelation(const Surface& domain, const Surface& codomain, HalfEdge preimage, HalfEdge image) : GenericRetriangulationDeformationRelation<Surface>(domain, codomain, Path{SaddleConnection<Surface>(domain, preimage)}, Path{SaddleConnection<Surface>(codomain, image)}) {
}

template <typename Surface>
GenericRetriangulationDeformationRelation<Surface>::GenericRetriangulationDeformationRelation(const Surface& domain, const Surface& codomain, Path<Surface> preimage, Path<Surface> image) : RetriangulationDeformationRelation<Surface>(domain, codomain), preimage(std::move(preimage)), image(std::move(image)) {
  LIBFLATSURF_ASSERT(this->preimage.begin() != this->preimage.end(), "preimage must be a non-trivial path");
  LIBFLATSURF_ASSERT(this->preimage.begin()->surface() == *this->domain, "preimage must live in the domain");
  LIBFLATSURF_ASSERT(this->image.begin() != this->image.end(), "preimage must be a non-trivial path");
  LIBFLATSURF_ASSERT(this->image.begin()->surface() == *this->codomain, "image must live in the codomain");
  LIBFLATSURF_ASSERT(this->domain->angle(Vertex::source(this->preimage.begin()->source(), *this->domain)) == this->codomain->angle(Vertex::source(this->image.begin()->source(), *this->codomain)), "Paths in domain and codomain cannot be equivalent since the total angle at their starting points do not match.");
  LIBFLATSURF_ASSERT(this->preimage.begin()->vector().ccw(this->image.begin()->vector()) == CCW::COLLINEAR, "Preimage and image are not collinear. They do not define a retriangulation.");
  LIBFLATSURF_ASSERT(this->preimage.begin()->vector().orientation(this->image.begin()->vector()) == ORIENTATION::SAME, "Preimage and image are not oriented identically. They do not define a retriangulation.");
}

template <typename Surface>
std::optional<Path<Surface>> GenericRetriangulationDeformationRelation<Surface>::operator()(const Path<Surface>& path) const {
  if (path.size() == 0)
    return Path<Surface>{};

  Path<Surface> pending = path;

  const HalfEdge source = pending.begin()->source();
  HalfEdge target = pending.begin()->target();
  auto vector = pending.begin()->vector();

  pending.pop_front();

  while (!pending.empty()) {
    auto vector_ = pending.begin()->vector();
    if (vector.ccw(vector_) == CCW::COLLINEAR && vector.orientation(vector_) == ORIENTATION::SAME && this->domain->angle(Vertex::source(pending.begin()->source(), *this->domain)) == 1) {
      // The first entries of the path are collinear and cross over a marked
      // point. Combine them into one (search) vector.
      vector += vector_;
      target = pending.begin()->target();
      pending.pop_front();
    }
  }

  const auto path_ = this->operator()(source, target, vector);
  if (!path_)
    return std::nullopt;

  const auto pending_ = this->operator()(pending);
  if (!pending_)
    return std::nullopt;

  return *path_ + *pending_;
}

template <typename Surface>
std::optional<Path<Surface>> GenericRetriangulationDeformationRelation<Surface>::operator()(HalfEdge source, HalfEdge target, const Vector<T>& vector) const {
  auto source_ = this->source(source, vector);
  if (!source_)
    return std::nullopt;

  auto target_ = this->source(target, -vector);
  if (!target_)
    return std::nullopt;

  // TODO: In some cases, e.g., when no vertices have been inserted, we can
  // speed this up since we do not need to search the saddle connection at all.

  Path<Surface> image;
  Vector<T> vector_;
  while(true) {
    const auto search = SaddleConnection<Surface>::counterclockwise(*this->codomain, SaddleConnection<Surface>(*this->codomain, *source_), Vertical<Surface>(*this->codomain, vector));

    vector_ += search;
    image += search;

    if (vector_ == vector)
      break;

    LIBFLATSURF_ASSERT((vector - vector_).orientation(vector) == ORIENTATION::SAME, "Partial path " << image << " was longer than its preimage " << vector << " from " << source << " to " << target << " but it can only be shorter when applying " << *this);
    LIBFLATSURF_ASSERT(this->codomain->angle(Vertex::source(search.target(), *this->codomain)) == 1, "Image of path crosses over a singularity.");

    source_ = this->codomain->nextAtVertex(search.target());
  }

  return image;
}

template <typename Surface>
std::optional<HalfEdge> GenericRetriangulationDeformationRelation<Surface>::source(HalfEdge source, const Vector<T>& vector) const {
  const auto relation = this->relation(Vertex::source(source, *this->domain));
  if (!relation)
    return std::nullopt;

  // These two saddle connections are equal but live in domain and codomain respectively.
  const auto& basepreimage = relation->first;
  const auto& baseimage = relation->second;

  // To reconstruct the path in the codomain we need to determine the angle
  // from `basepreimage` to `vector` and then reproduce the same angle in the
  // codomain from `baseimage`.
  const int angle = basepreimage.angle(source, vector);

  HalfEdge source_ = baseimage.source();
  {
    LIBFLATSURF_ASSERT(this->domain->angle(Vertex::source(source, *this->domain)) == this->codomain->angle(Vertex::source(source_, *this->codomain)), "Total angle at vertex must be the same in domain and codomain.");

    // Perform `angle` full turns of baseimage.
    source_ = turn(this->codomain, source_, baseimage.vector(), angle);

    // Now `baseimage.vector()` is in the sector next to `source_` which has
    // been turned `angle` times from its original value. Now we perform the
    // turn <2π from `basepreimage` to `(source, vector)`.
    source_ = turn(this->codomain, source_, baseimage.vector(), vector);
  }

  return source_;
}

template <typename Surface>
std::optional<std::pair<SaddleConnection<Surface>, SaddleConnection<Surface>>> GenericRetriangulationDeformationRelation<Surface>::relation(const Vertex& search) const {
  LIBFLATSURF_ASSERT(search == Vertex::source(*search.outgoing().begin(), *this->domain), "source " << search << " is not a vertex of " << *this->domain);

  if (relations.find(search) != relations.end())
    return relations.at(search);

  const auto insert = [&](const auto& a, const auto& b) {
    this->relations.insert({
        Vertex::source(a.source(), *this->domain),
        std::pair{a, b}});
  };

  insert(*preimage.begin(), *image.begin());
  insert(*(-preimage).begin(), *(-image).begin());

  for (const auto& connection : SaddleConnections<Surface>{*this->domain}.byLength()) {
    const auto source = Vertex::source(connection.source(), *this->domain);
    const auto target = Vertex::source(connection.target(), *this->domain);

    if (relations.find(source) == relations.end())
      continue;

    // This saddle connection starts from a known vertex so we can use it to
    // create a relation between domain and codomain.

    if (relations.find(target) == relations.end()) {
      // This saddle connection starts at an unknown vertex. It will give us a
      // new relation.

      // Reconstruct the same saddle connection in the codomain.
      HalfEdge cosource = relations.at(source).second.source();
      cosource = turn(this->codomain, cosource, relations.at(source).second.vector(), relations.at(source).first.angle(connection));
      cosource = turn(this->codomain, cosource, relations.at(source).second.vector(), connection.vector());

      const auto coconnections = SaddleConnections<Surface>{*this->codomain}.sector(cosource).sector(connection.vector(), connection.vector()).bound(Bound::upper(connection.vector()));
      if (coconnections.begin() == coconnections.end()) {
        // The connection's target is a marked point that has been removed in the codomain.
        if (target == search)
          return std::nullopt;
      } else {
        insert(-connection, -*coconnections.begin());
      }
    }

    if (relations.find(search) != relations.end())
      return relations.at(search);
  }

  LIBFLATSURF_UNREACHABLE("Surface only had a finite number of saddle connections.");
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> GenericRetriangulationDeformationRelation<Surface>::clone() const {
  return std::make_unique<GenericRetriangulationDeformationRelation<Surface>>(*this);
}

template <typename Surface>
HalfEdge GenericRetriangulationDeformationRelation<Surface>::turn(const Surface& surface, HalfEdge source, Vector<T> from, const Vector<T>& to) {
  while(from.ccw(to) != CCW::COLLINEAR || from.orientation(to) != ORIENTATION::SAME) {
    if (from.ccw(to) == CCW::COUNTERCLOCKWISE) {
      if (surface.fromHalfEdge(surface.nextAtVertex(source)).ccw(to) == CCW::CLOCKWISE) {
        // The image is in the same sector as from, i.e., next to source.
        from = to;
        break;
      }
    }
    source = surface.nextAtVertex(source);
    from = surface.fromHalfEdge(source);
  }

  return source;
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> GenericRetriangulationDeformationRelation<Surface>::section() const {
  return std::make_unique<GenericRetriangulationDeformationRelation>(this->codomain, this->domain, this->image, this->preimage);
}

template <typename Surface>
bool GenericRetriangulationDeformationRelation<Surface>::trivial() const {
  if (this->domain != this->codomain)
    return false;

  return preimage == image;
}

template <typename Surface>
HalfEdge GenericRetriangulationDeformationRelation<Surface>::turn(const Surface& surface, HalfEdge source, Vector<T> direction, int angle) {
  for (int turn = 0; turn < angle; turn++) {
    while(direction.ccw(surface.fromHalfEdge(source)) != CCW::COUNTERCLOCKWISE)
      source = surface.nextAtVertex(source);
    while(direction.ccw(surface.fromHalfEdge(source)) != CCW::CLOCKWISE)
      source = surface.nextAtVertex(source);
    while(direction.ccw(surface.fromHalfEdge(source)) != CCW::COUNTERCLOCKWISE)
      source = surface.nextAtVertex(source);
    source = surface.previousAtVertex(source);
  }
  return source;
}

template <typename Surface>
std::ostream& GenericRetriangulationDeformationRelation<Surface>::operator>>(std::ostream& os) const {
  return os << this->domain << " → " << this->codomain << " given by " << this->preimage << " ≈ " << this->image;
}

}

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_STATIC), GenericRetriangulationDeformationRelation, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
