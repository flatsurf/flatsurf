/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2022 Julian Rüth
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

#include "../flatsurf/segment_iterator.hpp"

#include <ostream>

#include "../flatsurf/saddle_connection.hpp"
#include "impl/path.impl.hpp"
#include "impl/segment.impl.hpp"
#include "impl/segment_iterator.impl.hpp"
#include "util/assert.ipp"

namespace flatsurf {

template <typename Surface>
void SegmentIterator<Surface>::increment() {
  LIBFLATSURF_ASSERT(!self->parent->empty(), "cannot increment iterator into empty path");
  LIBFLATSURF_ASSERT(!self->end, "cannot increment end() iterator")
  self->position++;
  if (self->position == end(self->parent->self->path)) {
    self->position = begin(self->parent->self->path);
    self->turn++;
  }
}

template <typename Surface>
const Segment<Surface>& SegmentIterator<Surface>::dereference() const {
  LIBFLATSURF_ASSERT(!self->parent->empty(), "cannot dereference iterator into empty path");
  LIBFLATSURF_ASSERT(!self->end, "cannot dereference end() iterator");
  LIBFLATSURF_ASSERT(self->position != end(self->parent->self->path), "iterator in impossible end state");
  return *self->position;
}

template <typename Surface>
bool SegmentIterator<Surface>::equal(const SegmentIterator& other) const {
  if (self->end) {
    return other.self->end || other.self->turn;
  }
  if (other.self->end) {
    return self->turn;
  }
  return self->position == other.self->position;
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const SegmentIterator<Surface>&) {
  return os << "SegmentIterator()";
}

template <typename Surface>
ImplementationOf<SegmentIterator<Surface>>::ImplementationOf(const Path<Surface>* parent, const Position& position) :
  parent(parent),
  position(position) {
  if (position == std::end(parent->self->path)) {
    this->end = true;
    this->position = begin(parent->self->path);
  }
}

template <typename Surface>
SegmentIterator<Surface> ImplementationOf<SegmentIterator<Surface>>::make(const Path<Surface>& parent, const Position& position) {
  return SegmentIterator<Surface>{PrivateConstructor{}, &parent, position};
}
}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), SegmentIterator, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)

