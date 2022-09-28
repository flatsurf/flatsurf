/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2021-2022 Julian Rüth
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

#include "impl/generic_retriangulation_deformation_relation.hpp"

#include <ostream>
#include <unordered_set>

#include "../flatsurf/ccw.hpp"
#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/half_edge_set.hpp"
#include "../flatsurf/half_edge_set_iterator.hpp"
#include "../flatsurf/orientation.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/point.hpp"
#include "../flatsurf/path_iterator.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/saddle_connections_by_length.hpp"
#include "../flatsurf/saddle_connections_by_length_iterator.hpp"
#include "../flatsurf/saddle_connections_iterator.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/vertex.hpp"
#include "../flatsurf/vertical.hpp"
#include "util/assert.ipp"
#include "external/rx-ranges/include/rx/ranges.hpp"

namespace flatsurf {

template <typename Surface>
GenericRetriangulationDeformationRelation<Surface>::GenericRetriangulationDeformationRelation(const Surface& domain, const Surface& codomain, HalfEdge preimage, HalfEdge image) :
  GenericRetriangulationDeformationRelation<Surface>(domain, codomain, Path{SaddleConnection<Surface>(domain, preimage)}, Path{SaddleConnection<Surface>(codomain, image)}) {
}

template <typename Surface>
GenericRetriangulationDeformationRelation<Surface>::GenericRetriangulationDeformationRelation(const Surface& domain, const Surface& codomain, Path<Surface> preimage, Path<Surface> image) :
  DeformationRelation<Surface>(domain, codomain), preimage(std::move(preimage)), image(std::move(image)) {
  LIBFLATSURF_ASSERT(this->preimage.begin() != this->preimage.end(), "preimage must be a non-trivial path");
  LIBFLATSURF_ASSERT(this->preimage.begin()->surface() == *this->domain, "preimage must live in the domain");
  LIBFLATSURF_ASSERT(rx::zip(this->preimage, this->preimage | rx::skip_n(1)) | rx::all_of([&](const auto& connections) {
      return domain.angle(Vertex::source(std::get<0>(connections).target(), domain)) == 1
        && std::get<0>(connections).ccw(std::get<1>(connections)) == CCW::COLLINEAR
        && std::get<0>(connections).vector().orientation(std::get<1>(connections)) == ORIENTATION::SAME;
  }), "Preimage must be a sequence of collinear saddle connections only interrupted by marked points but it is not.");
  LIBFLATSURF_ASSERT(rx::zip(this->image, this->image | rx::skip_n(1)) | rx::all_of([&](const auto& connections) {
      return domain.angle(Vertex::source(std::get<0>(connections).target(), codomain)) == 1
        && std::get<0>(connections).ccw(std::get<1>(connections)) == CCW::COLLINEAR
        && std::get<0>(connections).vector().orientation(std::get<1>(connections)) == ORIENTATION::SAME;
  }), "Image must be a sequence of collinear saddle connections only interrupted by marked points but it is not.");
  LIBFLATSURF_ASSERT(this->image.begin() != this->image.end(), "image must be a non-trivial path");
  LIBFLATSURF_ASSERT(this->image.begin()->surface() == *this->codomain, "image must live in the codomain");
  LIBFLATSURF_ASSERT(this->domain->angle(Vertex::source(this->preimage.begin()->source(), *this->domain)) == this->codomain->angle(Vertex::source(this->image.begin()->source(), *this->codomain)), "Paths in domain and codomain must be equivalent but the total angle at their starting points do not match.");
  LIBFLATSURF_ASSERT(this->preimage.begin()->vector().ccw(this->image.begin()->vector()) == CCW::COLLINEAR, "Preimage and image must be collinear but they are not.");
  LIBFLATSURF_ASSERT(this->preimage.begin()->vector().orientation(this->image.begin()->vector()) == ORIENTATION::SAME, "Preimage and image are not oriented identically. They do not define a retriangulation.");
  LIBFLATSURF_ASSERT((this->preimage | rx::transform([](const auto& connection) { return connection.vector(); }) | rx::sum())
      == (this->image | rx::transform([](const auto& connection) { return connection.vector(); }) | rx::sum()), "Preimage and image must describe the same saddle connection up to marked points but the sum of their vectors do not match.")
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
Point<Surface> GenericRetriangulationDeformationRelation<Surface>::operator()(const Point<Surface>& point) const {
  if (point.vertex()) {
    const auto A = *point.vertex();

    // We walk the domain surface trying to find a path to a vertex that
    // survived the deformation.
    std::unordered_set<Vertex> seen;

    const std::function<std::optional<Point<Surface>>(std::optional<HalfEdge>)> walk = [&](std::optional<HalfEdge> step) -> std::optional<Point<Surface>> {
      auto B = step ? Vertex::target(*step, *this->domain) : A;

      if (seen.find(B) != seen.end())
        return std::nullopt;

      seen.insert(B);

      if (!step) {
        const auto relation = this->relation(B);
        if (relation)
          // This vertex still exists in the codomain.
          return Point{*this->codomain, relation->second.source(), T(1), T(), T()};
      } else {
        const auto source = this->source(-*step, this->domain->fromHalfEdge(-*step));
        if (source)
          // This vertex still exists in the codomain.
          return Point{*this->codomain, *source, this->domain->fromHalfEdge(-*step)};
      }

      LIBFLATSURF_ASSERT(this->domain->angle(B) == 1, "vertex that disappears in a retriangulation must have been a marked point");

      // Walk recursively from the vertex.
      for (const auto next : B.outgoing()) {
        auto image = walk(next);
        if (image) {
          if (step)
            *image -= this->domain->fromHalfEdge(*step);
          return *image;
        }
      }

      return std::nullopt;
    };

    const auto image = walk(std::nullopt);

    LIBFLATSURF_ASSERT(image, "vertex is not connected to anything that exists in the codomain after deformation");

    return *image;
  } else {
    const HalfEdge face = point.face();
    const Vector<T> toPoint = point.vector(face);
    const auto source = this->source(face, toPoint);
    const auto base = Vertex::source(face, *this->domain);

    if (source)
      return Point{*this->codomain, *source, toPoint};

    LIBFLATSURF_ASSERT(this->domain->angle(base) == 1, "vertex that disappears in a retriangulation must have been a marked point");

    return this->operator()(Point{*this->domain, base}) + toPoint;
  }
}

template <typename Surface>
std::optional<Path<Surface>> GenericRetriangulationDeformationRelation<Surface>::operator()(HalfEdge source, HalfEdge target, const Vector<T>& vector) const {
  auto source_ = this->source(source, vector);
  if (!source_)
    return std::nullopt;

  auto target_ = this->source(target, -vector);
  if (!target_)
    return std::nullopt;

  // Note: In some cases, e.g., when no vertices have been inserted, we could
  // speed this up since we do not need to search the saddle connection at all.

  Path<Surface> image;
  Vector<T> vector_;
  while (true) {
    const auto search = SaddleConnection<Surface>::inSector(
      this->codomain,
      this->codomain->sector(*source_, this->codomain->fromHalfEdge(*source_), CCW::COUNTERCLOCKWISE, vector),
      vector);

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

  // These two saddle connections are equal but live in domain and codomain, respectively.
  const auto& basepreimage = relation->first;
  const auto& baseimage = relation->second;

  // To reconstruct the path in the codomain we need to determine the angle
  // from `basepreimage` to `vector` and then reproduce the same angle in the
  // codomain from `baseimage`.
  const int angle = basepreimage.angle(source, vector);

  HalfEdge source_ = baseimage.source();
  {
    LIBFLATSURF_ASSERT(this->domain->angle(Vertex::source(source, *this->domain)) == this->codomain->angle(Vertex::source(source_, *this->codomain)), "Total angle at vertex must be the same in domain and codomain but turn angle at " << Vertex::source(source, *this->domain) << " in " << *this->domain << " is " << this->domain->angle(Vertex::source(source, *this->domain)) << " whereas it is " << this->codomain->angle(Vertex::source(source_, *this->codomain)) << " at " << Vertex::source(source_, *this->codomain) << " in " << *this->codomain << ". The relation of these two surfaces is based upon " << this->preimage << " being equal to " << this->image);

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
    this->relations.insert({Vertex::source(a.source(), *this->domain),
        std::pair{a, b}});
  };

  insert(*preimage.begin(), *image.begin());
  insert(*(-preimage).begin(), *(-image).begin());

  for (const auto& connectionInDomain : SaddleConnections<Surface>{*this->domain}.byLength()) {
    const auto sourceInDomain = Vertex::source(connectionInDomain.source(), *this->domain);
    const auto targetInDomain = Vertex::source(connectionInDomain.target(), *this->domain);

    if (relations.find(sourceInDomain) == relations.end())
      continue;

    // This saddle connection starts from a known vertex so we can use it to
    // create a relation between domain and codomain.

    if (relations.find(targetInDomain) == relations.end()) {
      // This saddle connection ends at an unknown vertex. It will give us a
      // new relation.

      // Reconstruct the same saddle connection in the codomain.
      HalfEdge sourceInCodomain = relations.at(sourceInDomain).second.source();
      sourceInCodomain = turn(this->codomain, sourceInCodomain, relations.at(sourceInDomain).second.vector(), relations.at(sourceInDomain).first.angle(connectionInDomain));
      sourceInCodomain = turn(this->codomain, sourceInCodomain, relations.at(sourceInDomain).second.vector(), connectionInDomain.vector());

      const auto connectionsInCodomain = SaddleConnections<Surface>{*this->codomain}.sector(sourceInCodomain).sector(connectionInDomain.vector(), connectionInDomain.vector()).bound(Bound::upper(connectionInDomain.vector()));
      if (connectionsInCodomain.begin() == connectionsInCodomain.end()) {
        // The connection cannot be realized in the codomain because the
        // connection's target is a marked point that has been removed in the
        // codomain.
        if (targetInDomain == search)
          // That target is the vertex we are looking for but it does not exist
          // in the codomain anymore.
          return std::nullopt;

        continue;
      }

      const auto connectionInCodomain = *connectionsInCodomain.begin();
      if (connectionInCodomain.vector() != connectionInDomain.vector()) {
        // The connection cannot be realized in the codomain because it hits a
        // marked point before reaching the corresponding target.
        continue;
      }

      insert(-connectionInDomain, -connectionInCodomain);
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
  while (from.ccw(to) != CCW::COLLINEAR || from.orientation(to) != ORIENTATION::SAME) {
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
    while (direction.ccw(surface.fromHalfEdge(source)) != CCW::COUNTERCLOCKWISE)
      source = surface.nextAtVertex(source);
    while (direction.ccw(surface.fromHalfEdge(source)) != CCW::CLOCKWISE)
      source = surface.nextAtVertex(source);
    while (direction.ccw(surface.fromHalfEdge(source)) != CCW::COUNTERCLOCKWISE)
      source = surface.nextAtVertex(source);
    source = surface.previousAtVertex(source);
  }
  return source;
}

template <typename Surface>
std::ostream& GenericRetriangulationDeformationRelation<Surface>::operator>>(std::ostream& os) const {
  return os << this->domain << " → " << this->codomain << " given by " << this->preimage << " ≈ " << this->image;
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_STATIC), GenericRetriangulationDeformationRelation, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
