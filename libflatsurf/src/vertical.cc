/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2020 Julian Rüth
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

#include "../flatsurf/vertical.hpp"

#include <intervalxt/interval_exchange_transformation.hpp>
#include <intervalxt/label.hpp>
#include <unordered_set>

#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/flat_triangulation_collapsed.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/interval_exchange_transformation.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/vector.hpp"
#include "impl/vertical.impl.hpp"
#include "util/assert.ipp"

using std::ostream;
using namespace flatsurf;

namespace flatsurf {

template <typename Surface>
Vertical<Surface>::Vertical(const Surface& surface, const Vector<T>& vertical) :
  self(spimpl::make_impl<ImplementationOf<Vertical>>(surface, vertical)) {}

template <typename Surface>
std::vector<std::unordered_set<HalfEdge>> Vertical<Surface>::components() const {
  std::vector<std::unordered_set<HalfEdge>> components;
  std::unordered_set<HalfEdge> done;
  for (const auto& start : self->surface->halfEdges()) {
    if (done.find(start) != done.end())
      continue;
    std::unordered_set<HalfEdge> component;
    if (!ImplementationOf<Vertical>::visit(*this, start, component, [&](HalfEdge) { return true; })) {
      assert(false && "visit cannot fail without a predicate");
    }
    assert(component.size() && "visit cannot return an empty component");
    components.push_back(component);
    done.insert(component.begin(), component.end());
  }
  return components;
}

template <typename Surface>
bool Vertical<Surface>::large(HalfEdge e) const {
  auto length = [&](const HalfEdge edge) {
    auto ret = perpendicular(self->surface->fromHalfEdge(edge));
    return ret > 0 ? ret : -ret;
  };
  auto len = length(e);
  return len >= length(self->surface->nextInFace(e)) &&
         len >= length(self->surface->previousInFace(e)) &&
         len >= length(self->surface->nextInFace(-e)) &&
         len >= length(self->surface->previousInFace(-e));
}

template <typename Surface>
typename Surface::Coordinate Vertical<Surface>::perpendicular(const Vector<T>& v) const {
  return self->horizontal * v;
}

template <typename Surface>
typename Surface::Coordinate Vertical<Surface>::parallel(const Vector<T>& v) const {
  return self->vertical * v;
}

template <typename Surface>
bool Vertical<Surface>::perpendicular(HalfEdge e) const {
  return !static_cast<bool>(parallel(self->surface->fromHalfEdge(e)));
}

template <typename Surface>
bool Vertical<Surface>::parallel(HalfEdge e) const {
  return !static_cast<bool>(perpendicular(self->surface->fromHalfEdge(e)));
}

template <typename Surface>
typename Vertical<Surface>::TRIANGLE Vertical<Surface>::classifyFace(HalfEdge face) const {
  // Some of these cases are not possible if Surface is collapsed. However, the
  // more important optimization would be to cache the values of perpendicular
  // in a HalfEdgeMap.

  auto perp = perpendicular(self->surface->fromHalfEdge(face));
  auto a = perpendicular(self->surface->fromHalfEdge(self->surface->nextInFace(face)));
  auto b = perpendicular(self->surface->fromHalfEdge(self->surface->previousInFace(face)));

  if (self->surface->nextInFace(face) == self->surface->previousInFace(face)) {
    assert(perp + a == 0 && "face is not closed");
    return TRIANGLE::COLLAPSED_TO_TWO_FACES;
  }

  assert(perp + a + b == 0 && "face is not closed");

  if (perp == 0) {
    assert(a != 0 && b != 0 && "face cannot have two vertical edges");
    return classifyFace(self->surface->nextInFace(face));
  } else if (perp < 0) {
    return classifyFace(self->surface->nextInFace(face));
  } else {
    if (a == 0) {
      return TRIANGLE::RIGHT_VERTICAL;
    } else if (b == 0) {
      return TRIANGLE::LEFT_VERTICAL;
    } else if (a > 0) {
      return TRIANGLE::FORWARD;
    } else if (b > 0) {
      return TRIANGLE::FORWARD;
    } else {
      return TRIANGLE::BACKWARD;
    }
  }
}

template <typename Surface>
Vertical<Surface> Vertical<Surface>::operator-() const {
  return Vertical(self->surface, -vertical());
}

template <typename Surface>
const Surface& Vertical<Surface>::surface() const {
  return self->surface;
}

template <typename Surface>
bool Vertical<Surface>::operator==(const Vertical& rhs) const {
  return surface() == rhs.surface() && self->vertical == rhs.self->vertical;
}

template <typename Surface>
const Vector<typename Surface::Coordinate>& Vertical<Surface>::vertical() const {
  return self->vertical;
}

template <typename Surface>
const Vector<typename Surface::Coordinate>& Vertical<Surface>::horizontal() const {
  return self->horizontal;
}

template <typename Surface>
ostream& operator<<(ostream& os, const Vertical<Surface>& self) {
  return os << self.self->vertical;
}

template <typename Surface>
ImplementationOf<Vertical<Surface>>::ImplementationOf(const Surface& surface, const Vector<T>& vertical) :
  surface(surface),
  vertical(vertical),
  horizontal(-vertical.perpendicular()) {
  CHECK_ARGUMENT(vertical, "vertical must be non-zero");
}

template <typename Surface>
bool ImplementationOf<Vertical<Surface>>::visit(const Vertical& self, HalfEdge start, std::unordered_set<HalfEdge>& component, std::function<bool(HalfEdge)> visitor) {
  if (component.find(start) != component.end())
    return true;

  component.insert(start);

  if (!visitor(start))
    return false;

  // Do not cross vertical edges.
  if (self.parallel(start))
    return true;

  // Visit all connected half edges.
  return visit(self, -start, component, visitor) &&
         visit(self, self.self->surface->nextInFace(start), component, visitor) &&
         visit(self, self.self->surface->previousInFace(start), component, visitor);
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), Vertical, LIBFLATSURF_SURFACE_TYPES)
