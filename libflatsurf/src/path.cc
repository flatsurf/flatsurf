/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020-2022 Julian Rüth
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

#include "../flatsurf/path.hpp"

#include <fmt/format.h>

#include <algorithm>
#include <ostream>
#include <unordered_set>

#include "../flatsurf/ccw.hpp"
#include "../flatsurf/chain.hpp"
#include "../flatsurf/fmt.hpp"
#include "../flatsurf/orientation.hpp"
#include "../flatsurf/path_iterator.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/saddle_connections_iterator.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/vertex.hpp"
#include "external/rx-ranges/include/rx/ranges.hpp"
#include "impl/path.impl.hpp"
#include "impl/path_iterator.impl.hpp"
#include "impl/saddle_connection.impl.hpp"
#include "util/assert.ipp"

namespace flatsurf {

namespace {

// Orders saddle connections around a vertex in counter clockwise order starting from a base connection b.
template <typename Surface>
struct SaddleConnectionOrdering {
  SaddleConnectionOrdering(SaddleConnection<Surface> b) :
    b(std::move(b)) {}

  bool operator()(const SaddleConnection<Surface>& x, const SaddleConnection<Surface>& y) {
    if (x == y)
      return false;

    const int xangle = b.angle(x);
    const int yangle = b.angle(y);

    if (xangle < yangle)
      return true;
    if (xangle > yangle)
      return false;

    switch (x.ccw(y)) {
      case CCW::COUNTERCLOCKWISE:
        switch (b.ccw(x)) {
          case CCW::COUNTERCLOCKWISE:
            switch (b.ccw(y)) {
              case CCW::COUNTERCLOCKWISE:
                // b < x < y
                return true;
              case CCW::CLOCKWISE:
                // b < x < y
                return true;
              case CCW::COLLINEAR:
                // b < x < y
                return true;
              default:
                LIBFLATSURF_UNREACHABLE("Impossible configuration of saddle connections.");
            }
          case CCW::CLOCKWISE:
            switch (b.ccw(y)) {
              case CCW::COUNTERCLOCKWISE:
                // b < y < x
                return false;
              case CCW::CLOCKWISE:
                // b < x < y
                return true;
              case CCW::COLLINEAR:
                LIBFLATSURF_UNREACHABLE("Cannot turn from " << y << " clockwise to " << x);
              default:
                LIBFLATSURF_UNREACHABLE("Impossible configuration of saddle connections.");
            }
          case CCW::COLLINEAR:
            switch (b.ccw(y)) {
              case CCW::COUNTERCLOCKWISE:
                // b < x < y
                LIBFLATSURF_ASSERT(b == x, "Cannot turn counterclockwise from " << x << " to " << y);
                return true;
              case CCW::CLOCKWISE:
                // b < x < y
                return true;
              case CCW::COLLINEAR:
                LIBFLATSURF_UNREACHABLE("Cannot turn counterclockwise from " << x << " to " << y << " if they are both collinear to " << b);
              default:
                LIBFLATSURF_UNREACHABLE("Impossible configuration of saddle connections.");
            }
          default:
            LIBFLATSURF_UNREACHABLE("Impossible configuration of saddle connections.");
        }
      case CCW::CLOCKWISE:
        return !this->operator()(y, x);
      case CCW::COLLINEAR:
        switch (b.ccw(x)) {
          case CCW::COUNTERCLOCKWISE:
            // b < x < y
            LIBFLATSURF_ASSERT(b.ccw(y) == CCW::CLOCKWISE, "Since " << x << " is counterclockwise to " << b << " its opposite " << y << " must be clockwise.");
            return true;
          case CCW::CLOCKWISE:
            // b < y < x
            LIBFLATSURF_ASSERT(b.ccw(y) == CCW::COUNTERCLOCKWISE, "Since " << x << " is clockwise to " << b << " its opposite " << y << " must be counterclockwize.");
            return false;
          case CCW::COLLINEAR:
            LIBFLATSURF_ASSERT(x == b || y == b, "If " << x << " and " << y << " and " << b << " are collinear, two of them must be identical.");
            return x == b;
          default:
            LIBFLATSURF_UNREACHABLE("Impossible configuration of saddle connections.");
        }
      default:
        LIBFLATSURF_UNREACHABLE("Impossible configuration of saddle connections.");
    }
  }

 private:
  SaddleConnection<Surface> b;
};

// Return a maximally convex path that refines the path a → b which is turning clockwise.
template <typename Surface>
Path<Surface> tightenClockwise(const SaddleConnection<Surface>& a, const SaddleConnection<Surface>& b) {
  const auto& surface = a.surface();

  LIBFLATSURF_ASSERT((-a).ccw(b) == CCW::CLOCKWISE, "Can only tighten clockwise vertices but we are not turning clockwise from " << -a << " to " << b);

  // The list of points that are on the convex path.
  // We store these points as saddle connections starting at the point where
  // a and b meet. Going in counterclockwise direction.
  // Due to the enumeration order of these saddle connections, this list
  // might eventualy contain points that are not on the convex hull. These
  // are then filtered out later.
  auto rays = std::list{b, -a};

  // All the end points of segments on the convex path must come from saddle
  // connections that start at the point where a and b meet.
  auto connections = SaddleConnections<Surface>{surface}.sector(b, -a).bound(std::max(Bound::upper(b.vector()), Bound::upper(a.vector())));

  using std::begin;
  using std::end;

  for (auto connection = begin(connections); connection != end(connections); ++connection) {
    if (*connection == b)
      continue;

    const auto insertBefore = std::lower_bound(begin(rays), end(rays), *connection, SaddleConnectionOrdering<Surface>{b});
    LIBFLATSURF_ASSERT(insertBefore != end(rays) && insertBefore != begin(rays), "Connection " << *connection << " must be in the range (" << b << ", " << -a << "] since we have chosen the search sector that way.");

    LIBFLATSURF_ASSERT(*connection != *insertBefore, "Connections must not contain duplicates except for the inclusive sector end but " << *connection << " is not equal to " << b);

    auto insertAfter = insertBefore;
    --insertAfter;

    const auto boundary = insertBefore->vector() - insertAfter->vector();

    const auto extendsConvexHull = [&](const auto& connection) {
      const auto v = connection - insertAfter->vector();
      return boundary.ccw(v) != CCW::CLOCKWISE;
    };

    if (extendsConvexHull(connection->vector())) {
      // This connection points to a vertex that (likely) forms a part of the convex hull.
      rays.insert(insertBefore, *connection);
    } else {
      // This connection points to a vertex that is beyond the convex hull we
      // constructed until now, so it cannot be part of the convex hull.
      const auto target = connection->target();
      const bool ccwInsideBoundary = extendsConvexHull(connection->vector() + surface.fromHalfEdge(target));
      const bool cwInsideBoundary = extendsConvexHull(connection->vector() + surface.fromHalfEdge(surface.nextAtVertex(target)));
      if (!ccwInsideBoundary && !cwInsideBoundary) {
        connection.skip();
      } else if (!ccwInsideBoundary) {
        connection.skipSector(CCW::COUNTERCLOCKWISE);
      } else if (!cwInsideBoundary) {
        connection.skipSector(CCW::CLOCKWISE);
      }
    }
  }

  // The order in which saddle connections are enumerated by the saddle
  // conections iterator, might lead to some points actually not on the
  // convex hull. We filter them out with the customary monotone chain algorithm.
  std::vector<SaddleConnection<Surface>> hull;
  for (auto ray : rays) {
    while (hull.size() >= 2) {
      const auto previous = hull.rbegin()->vector() - (++hull.rbegin())->vector();
      const auto next = ray.vector() - (++hull.rbegin())->vector();
      if (previous.ccw(next) != CCW::COUNTERCLOCKWISE)
        break;
      hull.pop_back();
    }
    hull.push_back(ray);
  }

  // Build an actual path from the rays that survived the convex hull algorithm.
  std::vector<SaddleConnection<Surface>> path;

  for (auto j = ++begin(hull); j != end(hull); ++j) {
    auto i = j;
    --i;

    const auto chain = j->chain() - i->chain();
    const auto vector = static_cast<const Vector<typename Surface::Coordinate>&>(chain);
    path.push_back(SaddleConnection<Surface>(surface,
        surface.sector(i->target(), CCW::CLOCKWISE, vector),
        surface.sector(j->target(), CCW::COUNTERCLOCKWISE, -vector),
        chain));
  }

  return -Path<Surface>{path};
}

}  // namespace

template <typename Surface>
Path<Surface>::Path() noexcept :
  self(spimpl::make_impl<ImplementationOf<Path>>()) {}

template <typename Surface>
Path<Surface>::Path(const Segment& path) :
  Path(std::vector{path}) {}

template <typename Surface>
Path<Surface>::Path(const std::vector<Segment>& path) :
  self(spimpl::make_impl<ImplementationOf<Path>>(path)) {}

template <typename Surface>
Path<Surface>::operator const std::vector<Segment> &() const {
  return self->path;
}

template <typename Surface>
Path<Surface> Path<Surface>::operator-() const {
  std::vector<Segment> reversed;
  for (auto p = rbegin(self->path); p != rend(self->path); p++)
    reversed.push_back(-*p);
  return reversed;
}

template <typename Surface>
bool Path<Surface>::operator==(const Path& rhs) const {
  return self->path == rhs.self->path;
}

template <typename Surface>
bool Path<Surface>::closed() const {
  if (empty()) return true;
  return ImplementationOf<Path>::connected(*std::rbegin(self->path), *std::begin(self->path));
}

template <typename Surface>
bool Path<Surface>::simple() const {
  using std::begin, std::end;
  std::unordered_set<Segment> segments(begin(self->path), end(self->path));
  return segments.size() == self->path.size();
}

template <typename Surface>
Path<Surface> Path<Surface>::reversed() const {
  return self->path | rx::transform([&](const auto& connection) { return -connection; }) | rx::reverse() | rx::to_vector();
}

template <typename Surface>
bool Path<Surface>::empty() const {
  return self->path.empty();
}

template <typename Surface>
size_t Path<Surface>::size() const {
  return self->path.size();
}

template <typename Surface>
void Path<Surface>::push_front(const Segment& segment) {
  LIBFLATSURF_ASSERT(empty() || ImplementationOf<Path>::connected(segment, *begin()), "Path must be connected but " << segment << " does not precede " << *begin());
  self->path.insert(std::begin(self->path), segment);
}

template <typename Surface>
void Path<Surface>::push_back(const Segment& segment) {
  LIBFLATSURF_ASSERT(empty() || ImplementationOf<Path>::connected(*self->path.rbegin(), segment), "Path must be connected but " << *self->path.rbegin() << " does not precede " << segment);
  self->path.push_back(segment);
}

template <typename Surface>
Path<Surface>& Path<Surface>::operator+=(const Path& other) {
  for (auto& segment : other)
    push_back(segment);
  return *this;
}

template <typename Surface>
Path<Surface> Path<Surface>::operator+(const Path& other) const {
  auto path = *this;
  return path += other;
}

template <typename Surface>
void Path<Surface>::pop_back() {
  self->path.pop_back();
}

template <typename Surface>
void Path<Surface>::pop_front() {
  self->path.erase(self->path.begin());
}

template <typename Surface>
void Path<Surface>::splice(const PathIterator<Surface>& pos, Path& other) {
  LIBFLATSURF_ASSERT_ARGUMENT(this != &other, "Cannot splice path into itself");

  const auto at = pos == end() ? std::end(self->path) : pos.self->position;
  self->path.insert(at, std::begin(other.self->path), std::end(other.self->path));

  LIBFLATSURF_ASSERTIONS([&]() {
    for (auto segment = std::begin(self->path); segment != std::end(self->path); segment++) {
      LIBFLATSURF_ASSERT(segment + 1 == std::end(self->path) || ImplementationOf<Path>::connected(*segment, *(segment + 1)), "Path must be connected but " << *segment << " does not precede " << *(segment + 1));
    }
    return true;
  });

  other.self->path.clear();
}

template <typename Surface>
void Path<Surface>::splice(const PathIterator<Surface>& pos, Path&& other) {
  // This could be done more efficiently if it shows up in the profiler.
  splice(pos, other);
}

template <typename Surface>
typename Surface::Coordinate Path<Surface>::area() const {
  LIBFLATSURF_CHECK_ARGUMENT(closed(), "Area can only be computed for closed paths but " << *this << " is not closed.");
  return Vector<T>::area(*this | rx::transform([](const auto& connection) { return connection.vector(); }) | rx::to_vector());
}

template <typename Surface>
PathIterator<Surface> Path<Surface>::begin() const {
  return PathIterator<Surface>(PrivateConstructor{}, this, std::begin(self->path));
}

template <typename Surface>
PathIterator<Surface> Path<Surface>::end() const {
  return PathIterator<Surface>(PrivateConstructor{}, this, std::end(self->path));
}

template <typename Surface>
Path<Surface> Path<Surface>::tighten() const {
  std::list<SaddleConnection<Surface>> path;

  for (const auto& connection : self->path)
    path.push_back(connection);

  auto a = path.begin();

  // Replace (d, a, b, c) in path with (d, ...replacement, c) and return an iterator pointing to d.
  const auto replace = [](const auto& a, auto& path, const auto& replacement) {
    bool begin = a == path.begin();

    auto d = a;
    if (!begin)
      --d;

    auto b = a;
    ++b;

    auto c = b;
    ++c;

    path.erase(a);
    path.erase(b);

    for (auto&& connection : replacement)
      path.insert(c, connection);

    return begin ? path.begin() : d;
  };

  while (true) {
    if (a == path.end())
      break;

    auto b = a;
    ++b;

    if (b == path.end())
      break;

    LIBFLATSURF_ASSERT(Vertex::source(b->source(), b->surface()) == Vertex::source(a->target(), a->surface()), "Path " << *this << " became disconnected at " << *a << ", " << *b << " during tigthening.");

    // We will now try to replace the subpath a → b with a more tight version.
    if (*a == -*b) {
      // Remove trivial subpath a → -a.
      a = replace(a, path, std::vector<SaddleConnection<Surface>>{});
      continue;
    } else {
      // Try to replace the subpath a → b with a tightening starting a a's
      // source vertex and ending at b's target vertex.
      const CCW ccw = (-*a).ccw(*b);
      if (ccw == (-a->vector()).ccw(b->vector()) && ((ccw == CCW::COUNTERCLOCKWISE && (-*a).angle(*b) == 0) ||
                                                        (ccw == CCW::CLOCKWISE && (*b).angle(-*a) == 0))) {
        // The angle enclosed where a and b meet is less than π so the path can be tightened.
        if (ccw == CCW::CLOCKWISE)
          a = replace(a, path, tightenClockwise(*a, *b));
        else
          a = replace(a, path, -tightenClockwise(-*b, -*a));

        continue;
      }
    }

    ++a;
  }

  return std::vector<SaddleConnection<Surface>>{path.begin(), path.end()};
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const Path<Surface>& path) {
  return os << fmt::format("[{}]", fmt::join(path.self->path, " → "));
}

template <typename Surface>
ImplementationOf<Path<Surface>>::ImplementationOf() :
  path() {}

template <typename Surface>
ImplementationOf<Path<Surface>>::ImplementationOf(const std::vector<Segment>& path) {
  for (auto segment = begin(path); segment != end(path); segment++) {
    LIBFLATSURF_ASSERT(segment + 1 == end(path) || connected(*segment, *(segment + 1)), "Path must be connected but " << *segment << " does not precede " << *(segment + 1) << " either because they are connected to different vertices or because the turn from " << -*segment << " to " << *(segment + 1) << " is not turning clockwise in the range (0, 2π]");
    this->path.push_back(*segment);
  }
}

template <typename Surface>
bool ImplementationOf<Path<Surface>>::connected(const Segment& from, const Segment& to) {
  return Vertex::source(from.target(), from.surface()) == Vertex::source(to.source(), to.surface());
}
}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), Path, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
