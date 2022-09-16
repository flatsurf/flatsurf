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

#ifndef LIBFLATSURF_TEST_GENERATORS_SEGMENT_GENERATOR_HPP
#define LIBFLATSURF_TEST_GENERATORS_SEGMENT_GENERATOR_HPP

#include <memory>
#include <vector>

#include "../../flatsurf/segment.hpp"
#include "../../flatsurf/point.hpp"
#include "../../flatsurf/half_edge.hpp"
#include "surface_generator.hpp"
#include "../external/catch2/single_include/catch2/catch.hpp"

namespace flatsurf::test {

template <typename T>
class SegmentGenerator : public Catch::Generators::IGenerator<Segment<FlatTriangulation<T>>> {
  std::shared_ptr<const FlatTriangulation<T>> surface;
  HalfEdge face;

  enum class SEGMENT {
    VERTEX_TO_VERTEX_ALONG_EDGE = 0,
    // Everything from here requires division by integers.
    VERTEX_TO_EDGE_ALONG_EDGE = 1,
    EDGE_TO_EDGE_ALONG_EDGE = 2,
    EDGE_TO_FACE = 3,
    FACE_TO_FACE = 4,
    FACE_TO_FACE_ACROSS_EDGE = 5,
    LAST = 5,
  } state;
  
  Segment<FlatTriangulation<T>> current;

  Segment<FlatTriangulation<T>> make(SEGMENT kind) {
    switch (kind) {
      case SEGMENT::VERTEX_TO_EDGE_ALONG_EDGE:
      case SEGMENT::VERTEX_TO_EDGE_ALONG_EDGE:
      case SEGMENT::EDGE_TO_EDGE_ALONG_EDGE:
      case SEGMENT::EDGE_TO_FACE:
      case SEGMENT::FACE_TO_FACE:
      case SEGMENT::FACE_TO_FACE_ACROSS_EDGE:
      default:
        throw std::logic_error("not implemented: SegmentGenerator::make()");
    }
  }

 public:
  SegmentGenerator(std::shared_ptr<const FlatTriangulation<T>> surface, HalfEdge face) : surface(surface), face(face), state(SEGMENT::SOURCE), current(make(state)) {
  }

  const Segment<FlatTriangulation<T>>& get() const override {
    return current;
  }

  bool next() override {
    if constexpr (!hasFractions<T>)
      // Only the first segment can be created without using any division.
      state = SEGMENT::LAST;

    if (state == SEGMENT::LAST)
      return false;

    state = static_cast<SEGMENT>(static_cast<int>(state) + 1);

    current = make(state);

    return true;
  }
};

template <typename T>
Catch::Generators::GeneratorWrapper<Segment<FlatTriangulation<T>>> segments(NamedSurface<T> surface, const HalfEdge face) {
  return Catch::Generators::GeneratorWrapper<Segment<FlatTriangulation<T>>>(std::unique_ptr<Catch::Generators::IGenerator<Segment<FlatTriangulation<T>>>>(new SegmentGenerator<T>(surface, face)));
}

}

#endif
