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

#include "../flatsurf/path_iterator.hpp"

#include "../flatsurf/saddle_connection.hpp"
#include "impl/path.impl.hpp"
#include "impl/path_iterator.impl.hpp"
#include "util/assert.ipp"

namespace flatsurf {

template <typename Surface>
void PathIterator<Surface>::increment() {
  ASSERT(!impl->parent->empty(), "cannot increment iterator into empty path");
  ASSERT(!impl->end, "cannot increment end() iterator")
  impl->position++;
  if (impl->position == end(impl->parent->impl->path)) {
    impl->position = begin(impl->parent->impl->path);
    impl->turn++;
  }
}

template <typename Surface>
const SaddleConnection<Surface>& PathIterator<Surface>::dereference() const {
  ASSERT(!impl->parent->empty(), "cannot dereference iterator into empty path");
  ASSERT(!impl->end, "cannot dereference end() iterator");
  ASSERT(impl->position != end(impl->parent->impl->path), "iterator in impossible end state");
  return *impl->position;
}

template <typename Surface>
bool PathIterator<Surface>::equal(const PathIterator& other) const {
  if (impl->end) {
    return other.impl->end || other.impl->turn;
  }
  if (other.impl->end) {
    return impl->turn;
  }
  return impl->position == other.impl->position;
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const PathIterator<Surface>&) {
  return os << "PathIterator()";
}

template <typename Surface>
ImplementationOf<PathIterator<Surface>>::ImplementationOf(const Path<Surface>* parent, const Position& position) :
  parent(parent),
  position(position) {
  if (position == std::end(parent->impl->path)) {
    this->end = true;
    this->position = begin(parent->impl->path);
  }
}
}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), PathIterator, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
