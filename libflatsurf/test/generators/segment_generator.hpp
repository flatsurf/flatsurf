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
#include "../../flatsurf/vertex.hpp"
#include "surface_generator.hpp"
#include "../external/catch2/single_include/catch2/catch.hpp"

namespace flatsurf::test {

template <typename T>
class SegmentGenerator : public Catch::Generators::IGenerator<Segment<FlatTriangulation<T>>> {
  std::shared_ptr<const FlatTriangulation<T>> surface;
  HalfEdge face;

  enum class SEGMENT {
    VERTEX_TO_VERTEX_ALONG_EDGE = 0,
    VERTEX_TO_VERTEX_ALONG_EDGE_OVERLAPPING = 1,
    // Everything from here requires division by integers.
    VERTEX_TO_EDGE_ALONG_EDGE = 2,
    EDGE_TO_EDGE_ALONG_EDGE = 3,
    EDGE_TO_FACE = 4,
    FACE_TO_FACE = 5,
    FACE_TO_FACE_ACROSS_EDGE = 6,
    LAST = 6,
  } state;
  
  Segment<FlatTriangulation<T>> current;

  std::optional<Segment<FlatTriangulation<T>>> make(SEGMENT kind) {
    switch (kind) {
      case SEGMENT::VERTEX_TO_VERTEX_ALONG_EDGE:
        return SaddleConnection<FlatTriangulation<T>>(*surface, face).segment();
      case SEGMENT::VERTEX_TO_VERTEX_ALONG_EDGE_OVERLAPPING:
        if (surface->angle(Vertex::target(face, *surface)) != 0)
          return std::nullopt;
        return Segment<FlatTriangulation<T>>(
            Point(*surface, Vertex::source(face, *surface)),
            face,
            2 * surface->fromHalfEdge(face));
      case SEGMENT::VERTEX_TO_EDGE_ALONG_EDGE:
        if constexpr (!hasFractions<T>)
          return std::nullopt;
        return Segment<FlatTriangulation<T>>(
            Point(*surface, Vertex::source(face, *surface)),
            face,
            surface->fromHalfEdge(face) / 2);
      case SEGMENT::EDGE_TO_EDGE_ALONG_EDGE:
        {
          if constexpr (!hasFractions<T>)
            return std::nullopt;
          const auto v = surface->fromHalfEdge(face) / 4;
          return Segment<FlatTriangulation<T>>(Point(*surface, face, v), v);
        }
      case SEGMENT::EDGE_TO_FACE:
        {
          if constexpr (!hasFractions<T>)
            return std::nullopt;
          const auto p = surface->fromHalfEdge(face) / 2;
          const auto v = -surface->fromHalfEdge(face) / 4 + surface->fromHalfEdge(surface->nextAtVertex(face)) / 4;
          return Segment<FlatTriangulation<T>>(
              Point(*surface, face, p),
              v);
        }
      case SEGMENT::FACE_TO_FACE:
        {
          if constexpr (!hasFractions<T>)
            return std::nullopt;
          const auto v = surface->fromHalfEdge(face) / 8 + surface->fromHalfEdge(surface->nextAtVertex(face)) / 8;
          return Segment<FlatTriangulation<T>>(Point(*surface, face, v), v);
        }
      case SEGMENT::FACE_TO_FACE_ACROSS_EDGE:
        {
          if constexpr (!hasFractions<T>)
            return std::nullopt;
          const auto p = surface->fromHalfEdge(face) / 4 + surface->fromHalfEdge(surface->nextAtVertex(face)) / 4;
          const auto q = surface->fromHalfEdge(face) / 4 + surface->fromHalfEdge(surface->previousAtVertex(face)) / 4;
          return Segment<FlatTriangulation<T>>(Point(*surface, face, p), -p + q);
        }
      default:
        throw std::logic_error("not implemented: SegmentGenerator::make()");
    }
  }

 public:
  SegmentGenerator(std::shared_ptr<const FlatTriangulation<T>> surface, HalfEdge face) : surface(surface), face(face), state(SEGMENT::VERTEX_TO_VERTEX_ALONG_EDGE), current(*make(state)) {
  }

  const Segment<FlatTriangulation<T>>& get() const override {
    return current;
  }

  bool next() override {
    while(state != SEGMENT::LAST) {
      state = static_cast<SEGMENT>(static_cast<int>(state) + 1);

      const auto segment = make(state);
      if (segment) {
        current = *segment;
        return true;
      }
    }

    return false;
  }
};

template <typename T>
Catch::Generators::GeneratorWrapper<Segment<FlatTriangulation<T>>> segments(NamedSurface<T> surface, const HalfEdge face) {
  return Catch::Generators::GeneratorWrapper<Segment<FlatTriangulation<T>>>(std::unique_ptr<Catch::Generators::IGenerator<Segment<FlatTriangulation<T>>>>(new SegmentGenerator<T>(surface, face)));
}

}

#endif
