/* ********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Vincent Delecroix
 *        Copyright (C) 2019 Julian Rüth
 *
 *  flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  flatsurf is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with flatsurf. If not, see <https://www.gnu.org/licenses/>.
 * *******************************************************************/

#ifndef LIBFLATSURF_CEREAL_HPP
#define LIBFLATSURF_CEREAL_HPP

#include <cereal/types/map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/vector.hpp>

#include "flatsurf/flat_triangulation_combinatorial.hpp"
#include "flatsurf/half_edge.hpp"
#include "flatsurf/permutation.hpp"
#include "flatsurf/vertex.hpp"

#include "flatsurf/flat_triangulation.hpp"
#include "flatsurf/half_edge_map.hpp"
#include "flatsurf/interval_exchange_transformation.hpp"
#include "flatsurf/saddle_connection.hpp"
#include "flatsurf/saddle_connections.hpp"
#include "flatsurf/vector.hpp"
#include "flatsurf/vector_along_triangulation.hpp"

namespace flatsurf {

template <typename Archive>
void Vertex::save(Archive& archive) const {
  archive(cereal::make_nvp("start", representative));
}

template <typename Archive>
void Vertex::load(Archive& archive) {
  archive(cereal::make_nvp("start", representative));
}

template <typename Archive>
int HalfEdge::save_minimal(Archive&) const {
  return id;
}

template <typename Archive>
void HalfEdge::load_minimal(Archive&, const int& value) {
  id = value;
}

template <typename Archive, typename T>
void save(Archive& archive, const Permutation<T>& self) {
  archive(cereal::make_nvp("cycles", self.cycles()));
}

template <typename Archive, typename T>
void load(Archive& archive, Permutation<T>& self) {
  std::vector<std::vector<T>> cycles;
  archive(cereal::make_nvp("cycles", cycles));
  self = Permutation(cycles);
}

template <typename Archive>
void FlatTriangulationCombinatorial::save(Archive& archive) const {
  std::vector<std::pair<HalfEdge, HalfEdge>> vertices;
  for (auto& e : halfEdges()) {
    vertices.push_back(std::pair(e, nextAtVertex(e)));
  }
  archive(cereal::make_nvp("vertices", Permutation<HalfEdge>(vertices)));

  std::vector<std::pair<HalfEdge, HalfEdge>> faces;
  for (auto& e : halfEdges()) {
    faces.push_back(std::pair(e, nextInFace(e)));
  }
  archive(cereal::make_nvp("faces", Permutation<HalfEdge>(vertices)));
}

template <typename Archive>
void FlatTriangulationCombinatorial::load(Archive& archive) {
  Permutation<HalfEdge> vertices;
  archive(cereal::make_nvp("vertices", vertices));
  *this = FlatTriangulationCombinatorial(vertices);
}

template <typename T>
template <typename Archive>
void FlatTriangulation<T>::save(Archive& archive) const {
  archive(cereal::make_nvp("combinatorial", *static_cast<const FlatTriangulationCombinatorial*>(this)->clone()));

  std::map<HalfEdge, typename FlatTriangulation<T>::Vector> vectors;
  for (auto& edge : halfEdges())
    vectors[edge] = fromEdge(edge);

  archive(cereal::make_nvp("vectors", vectors));
}

template <typename T>
template <typename Archive>
void FlatTriangulation<T>::load(Archive& archive) {
  FlatTriangulationCombinatorial combinatorial;
  archive(cereal::make_nvp("combinatorial", combinatorial));
  std::cout << combinatorial << std::endl;
  std::map<HalfEdge, typename FlatTriangulation<T>::Vector> map;
  archive(cereal::make_nvp("vectors", map));

  auto vectors = HalfEdgeMap<typename flatsurf::FlatTriangulation<T>::Vector>(&combinatorial, [](auto&, auto, auto&) {});
  for (auto& v : map)
    vectors.set(v.first, v.second);

  *this = FlatTriangulation<long long>(std::move(std::move(combinatorial)), std::move(vectors));
}

template <typename T>
template <typename Archive>
void Vector<T>::save(Archive& archive) const {
  archive(cereal::make_nvp("x", x()));
  archive(cereal::make_nvp("y", y()));
}

template <typename T>
template <typename Archive>
void Vector<T>::load(Archive& archive) {
  typename Vector<T>::Coordinate x, y;
  archive(cereal::make_nvp("x", x));
  archive(cereal::make_nvp("y", y));
  *this = Vector<T>(x, y);
}

template <typename Surface>
template <typename Archive>
void SaddleConnection<Surface>::save(Archive& archive) const {
  archive(cereal::make_nvp("surface", std::static_pointer_cast<const Surface>(surface().shared_from_this())));
  archive(cereal::make_nvp("source", source()));
  archive(cereal::make_nvp("target", target()));
  archive(cereal::make_nvp("vector", vector()));
  archive(cereal::make_nvp("crossings", crossings()));
}

template <typename Surface>
template <typename Archive>
void SaddleConnection<Surface>::load(Archive& archive) {
  std::shared_ptr<Surface> surface;
  archive(cereal::make_nvp("surface", surface));
  HalfEdge source, target;
  archive(cereal::make_nvp("source", source));
  archive(cereal::make_nvp("target", target));
  typename SaddleConnection<Surface>::Vector vector;
  archive(cereal::make_nvp("vector", vector));

  *this = SaddleConnection<Surface>(surface, source, target, vector);
}

}  // namespace flatsurf

#endif
