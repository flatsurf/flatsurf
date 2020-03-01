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

#include <algorithm>
#include <ostream>
#include <unordered_set>

#include <fmt/format.h>

#include "../flatsurf/ccw.hpp"
#include "../flatsurf/fmt.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/path_iterator.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/vertex.hpp"

#include "impl/path.impl.hpp"
#include "impl/path_iterator.impl.hpp"

#include "external/rx-ranges/include/rx/ranges.hpp"

#include "util/assert.ipp"

namespace flatsurf {

template <typename Surface>
Path<Surface>::Path() :
  impl(spimpl::make_impl<Implementation>()) {}

template <typename Surface>
Path<Surface>::Path(const std::vector<Segment>& path) :
  impl(spimpl::make_impl<Implementation>(path)) {}

template <typename Surface>
Path<Surface>::operator const std::vector<Segment>&() const {
  return impl->path;
}

template <typename Surface>
bool Path<Surface>::operator==(const Path& rhs) const {
  return impl->path == rhs.impl->path;
}

template <typename Surface>
bool Path<Surface>::closed() const {
  if (empty()) return true;
  return Implementation::connected(impl->path[size() - 1], impl->path[0]);
}

template <typename Surface>
bool Path<Surface>::simple() const {
  using std::begin, std::end;
  std::unordered_set<Segment> segments(begin(impl->path), end(impl->path));
  return segments.size() == impl->path.size();
}

template <typename Surface>
Path<Surface> Path<Surface>::reversed() const {
  return impl->path | rx::transform([&](const auto& connection) { return -connection; }) | rx::reverse() | rx::to_vector();
}

template <typename Surface>
bool Path<Surface>::empty() const {
  return impl->path.empty();
}

template <typename Surface>
size_t Path<Surface>::size() const {
  return impl->path.size();
}

template <typename Surface>
void Path<Surface>::push_front(const Segment& segment) {
  ASSERT(empty() || Implementation::connected(segment, *begin()), "Path must be connected but " << segment << " does not precede " << *begin() << " either because they are connected to different vertices or because the turn from " << -segment << " to " << *begin() << " is not turning clockwise in the range (0, 2π]");
  impl->path.insert(std::begin(impl->path), segment);
}

template <typename Surface>
void Path<Surface>::push_back(const Segment& segment) {
  ASSERT(empty() || Implementation::connected(*impl->path.rbegin(), segment), "Path must be connected but " << *impl->path.rbegin() << " does not precede " << segment << " either because they are connected to different vertices or because the turn from " << -*impl->path.rbegin() << " to " << segment << " is not turning clockwise in the range (0, 2π]");
  impl->path.push_back(segment);
}

template <typename Surface>
void Path<Surface>::splice(const PathIterator<Surface>& pos, Path& other) {
  ASSERT_ARGUMENT(this != &other, "Cannot splice path into itself");

  const auto at = pos == end() ? std::end(impl->path) : pos.impl->position;
  impl->path.insert(at, std::begin(other.impl->path), std::end(other.impl->path));

  ASSERTIONS([&]() {
    for (auto segment = std::begin(impl->path); segment != std::end(impl->path); segment++) {
      ASSERT(segment + 1 == std::end(impl->path) || Implementation::connected(*segment, *(segment + 1)), "Path must be connected but " << *segment << " does not precede " << *(segment + 1) << " either because they are connected to different vertices or because the turn from " << -*segment << " to " << *(segment + 1) << " is not turning clockwise in the range (0, 2π]");
    }
    return true;
  });

  other.impl->path.clear();
}

template <typename Surface>
template <typename ...Args>
PathIterator<Surface>::PathIterator(PrivateConstructor, Args&& ... args) :
  impl(spimpl::make_impl<Implementation>(std::forward<Args>(args)...)) {}

template <typename Surface>
PathIterator<Surface> Path<Surface>::begin() const {
  return PathIterator<Surface>(PrivateConstructor{}, this, std::begin(impl->path));
}

template <typename Surface>
PathIterator<Surface> Path<Surface>::end() const {
  return PathIterator<Surface>(PrivateConstructor{}, this, std::end(impl->path));
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const Path<Surface>& path) {
  return os << fmt::format("{}", fmt::join(path.impl->path, "→"));
}

template <typename Surface>
Implementation<Path<Surface>>::Implementation() :
  path() {}

template <typename Surface>
Implementation<Path<Surface>>::Implementation(const std::vector<Segment>& path) {
  for (auto segment = begin(path); segment != end(path); segment++) {
    ASSERT(segment + 1 == end(path) || connected(*segment, *(segment + 1)), "Path must be connected but " << *segment << " does not precede " << *(segment + 1) << " either because they are connected to different vertices or because the turn from " << -*segment << " to " << *(segment + 1) << " which is not turning clockwise in the range (0, 2π]");
    if (!this->path.empty() && *segment == -*(end(this->path) - 1)) { 
      this->path.pop_back();
    } else {
      this->path.push_back(*segment);
    }
  }
}

template <typename Surface>
bool Implementation<Path<Surface>>::connected(const Segment& a, const Segment& b) {
  const auto& surface = a.surface();

  const auto from = -a;
  const auto to = b;

  if (Vertex::source(from.source(), surface) != Vertex::source(to.source(), surface))
    return false;

  if (from.source() == to.source()) {
    // If from and to are in the same sector, we essentially check whether
    // we have to turn clockwise between them.
    if (from.vector().ccw(to.vector()) == CCW::CLOCKWISE) {
      return true;
    } else {
      return surface.angle(Vertex::source(from.source(), surface)) == 1;
    }
  } else {
    // If from and to are not in the same sector, we go through the sectors until we hit to.source().
    // We keep track how far sector is turned from from.vector() in multiples of π.
    int turn = 0;
    for(auto sector = surface.previousAtVertex(from.source()); turn != 2; sector = surface.previousAtVertex(sector)) {
      if (turn == 0) {
        if (from.vector().ccw(surface.fromEdge(sector)) != CCW::CLOCKWISE) {
          turn = 1;
        }
      } else if (turn == 1) {
        if (from.vector().ccw(surface.fromEdge(sector)) == CCW::CLOCKWISE) {
          turn = 2;
        }
      }

      if (sector == to.source()) {
        if (turn < 2) {
          // The entire sector is at most 2π from the "from" vector so "to" must be within a 2π turn.
          return true;
        } else {
          // Parts of the sector are more than a 2π turn from the "from" vector.
          return from.vector().ccw(to.vector()) != CCW::CLOCKWISE;
        }
      }
    }
    return false;
  }
}
}

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), Path, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
