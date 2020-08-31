/* ********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Vincent Delecroix
 *        Copyright (C) 2019-2020 Julian Rüth
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

#include <e-antic/renfxx_cereal.h>
#include <fmt/format.h>
#include <fmt/ostream.h>

#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/vector.hpp>
#include <exact-real/cereal.hpp>

#include "bound.hpp"
#include "chain.hpp"
#include "edge.hpp"
#include "flat_triangulation.hpp"
#include "flat_triangulation_combinatorial.hpp"
#include "half_edge.hpp"
#include "half_edge_set.hpp"
#include "permutation.hpp"
#include "saddle_connection.hpp"
#include "saddle_connections.hpp"
#include "vector.hpp"
#include "vertex.hpp"

namespace flatsurf {

namespace {

template <typename T>
struct ReplacementSerialization {
  static auto serializable(const T& value) {
    if constexpr (std::is_same_v<T, mpz_class>) {
      return value.get_str();
    } else if constexpr (std::is_same_v<T, mpq_class>) {
      return value.get_str();
    } else {
      return value;
    }
  }

  template <typename S>
  static auto deserializable(const S& value) {
    if constexpr (std::is_same_v<T, mpz_class>) {
      return mpz_class(value);
    } else if constexpr (std::is_same_v<T, mpq_class>) {
      return mpq_class(value);
    } else {
      return value;
    }
  }

  template <typename Archive>
  static void save(Archive& archive, const std::string key, const T& value) {
    archive(cereal::make_nvp(key, serializable(value)));
  }

  template <typename Archive>
  static void load(Archive& archive, const std::string key, T& value) {
    decltype(serializable(std::declval<T>())) s;
    archive(cereal::make_nvp(key, s));
    value = deserializable(s);
  }
};

}  // namespace

template <typename Archive>
int HalfEdge::save_minimal(Archive&) const {
  return id();
}

template <typename Archive>
void HalfEdge::load_minimal(Archive&, const int& value) {
  *this = HalfEdge(value);
}

template <typename Archive>
int Edge::save_minimal(Archive&) const {
  return positive().id();
}

template <typename Archive>
void Edge::load_minimal(Archive&, const int& value) {
  *this = Edge(HalfEdge(value));
}

template <typename Archive>
void Bound::save(Archive& archive) const {
  ReplacementSerialization<mpz_class>::save(archive, "square", square);
}

template <typename Archive>
void Bound::load(Archive& archive) {
  ReplacementSerialization<mpz_class>::load(archive, "square", square);
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

template <typename T>
template <typename Archive>
void Vector<T>::save(Archive& archive) const {
  ReplacementSerialization<T>::save(archive, "x", x());
  ReplacementSerialization<T>::save(archive, "y", y());
}

template <typename T>
template <typename Archive>
void Vector<T>::load(Archive& archive) {
  T x, y;
  ReplacementSerialization<T>::load(archive, "x", x);
  ReplacementSerialization<T>::load(archive, "y", y);
  *this = Vector<T>(x, y);
}

template <typename T>
struct Serialization {
  template <typename Archive>
  void save(Archive&, const T&) {
    static_assert(!std::is_same_v<T, T>, "no Serialization specialization for this type defined");
  }

  template <typename Archive>
  void load(Archive&, T&) {
    static_assert(!std::is_same_v<T, T>, "no Serialization specialization for this type defined");
  }
};

template <>
struct Serialization<FlatTriangulationCombinatorial> {
  template <typename Archive>
  void save(Archive& archive, const FlatTriangulationCombinatorial& self) {
    std::vector<std::pair<HalfEdge, HalfEdge>> vertices;
    for (auto& e : self.halfEdges()) {
      vertices.push_back(std::pair(e, self.nextAtVertex(e)));
    }
    archive(cereal::make_nvp("vertices", Permutation<HalfEdge>(vertices)));

    std::vector<std::pair<HalfEdge, HalfEdge>> faces;
    for (auto& e : self.halfEdges()) {
      faces.push_back(std::pair(e, self.nextInFace(e)));
    }
    archive(cereal::make_nvp("faces", Permutation<HalfEdge>(vertices)));
  }

  template <typename Archive>
  void load(Archive& archive, FlatTriangulationCombinatorial& self) {
    Permutation<HalfEdge> vertices;
    archive(cereal::make_nvp("vertices", vertices));
    self = FlatTriangulationCombinatorial(vertices);
  }
};

template <typename T>
struct Serialization<FlatTriangulation<T>> {
  template <typename Archive>
  void save(Archive& archive, const FlatTriangulation<T>& self) {
    archive(cereal::make_nvp("combinatorial", static_cast<const FlatTriangulationCombinatorial&>(self)));

    std::unordered_map<HalfEdge, Vector<T>> vectors;
    for (auto& edge : self.halfEdges())
      vectors[edge] = self.fromEdge(edge);

    archive(cereal::make_nvp("vectors", vectors));
  }

  template <typename Archive>
  void load(Archive& archive, FlatTriangulation<T>& self) {
    FlatTriangulationCombinatorial combinatorial;
    archive(cereal::make_nvp("combinatorial", combinatorial));
    std::unordered_map<HalfEdge, Vector<T>> map;
    archive(cereal::make_nvp("vectors", map));

    self = FlatTriangulation<T>(std::move(std::move(combinatorial)), [&](HalfEdge e) { return map.at(e); });
  }
};

template <typename Surface>
struct Serialization<Chain<Surface>> {
  template <typename Archive>
  void save(Archive& archive, const Chain<Surface>& self) {
    archive(cereal::make_nvp("surface", self.surface().shared_from_this()));

    std::vector<std::string> coefficients;
    for (auto edge : self.surface().edges())
      coefficients.push_back(fmt::format("{}", self[edge]));
    archive(cereal::make_nvp("coefficients", coefficients));
  }

  template <typename Archive>
  void load(Archive& archive, Chain<Surface>& self) {
    std::shared_ptr<const Surface> surface;
    archive(cereal::make_nvp("surface", surface));
    std::vector<std::string> coefficients;
    archive(cereal::make_nvp("coefficients", coefficients));

    self = Chain(surface);
    for (size_t i = 0; i < coefficients.size(); i++)
      self += ((Chain<Surface>(surface) += surface->edges()[i].positive()) *= mpz_class(coefficients[i]));
  }
};

template <typename Surface>
struct Serialization<SaddleConnection<Surface>> {
  template <typename Archive>
  void save(Archive& archive, const SaddleConnection<Surface>& self) {
    archive(cereal::make_nvp("surface", std::static_pointer_cast<const Surface>(self.surface().shared_from_this())));
    archive(cereal::make_nvp("source", self.source()));
    archive(cereal::make_nvp("target", self.target()));
    archive(cereal::make_nvp("chain", static_cast<Chain<Surface>>(self)));
    archive(cereal::make_nvp("crossings", self.crossings()));
  }

  template <typename Archive>
  void load(Archive& archive, SaddleConnection<Surface>& self) {
    std::shared_ptr<Surface> surface;
    archive(cereal::make_nvp("surface", surface));
    HalfEdge source, target;
    archive(cereal::make_nvp("source", source));
    archive(cereal::make_nvp("target", target));
    Chain<Surface> chain(surface);
    archive(cereal::make_nvp("chain", chain));

    self = SaddleConnection<Surface>(surface, source, target, chain);
  }
};

template <>
struct Serialization<Vertex> {
  template <typename Archive>
  void save(Archive& archive, const Vertex& self) {
    auto outgoing = self.outgoing();
    archive(cereal::make_nvp("outgoing", std::vector<HalfEdge>(begin(outgoing), end(outgoing))));
  }

  template <typename Archive>
  void load(Archive& archive, Vertex& self) {
    std::vector<HalfEdge> outgoing;
    archive(cereal::make_nvp("outgoing", outgoing));
    self = Vertex(PrivateConstructor{}, HalfEdgeSet(outgoing));
  }
};

template <typename Surface>
struct Serialization<Vertical<Surface>> {
  template <typename Archive>
  void save(Archive& archive, const Vertical<Surface>& self) {
    archive(cereal::make_nvp("surface", self.surface()));
    archive(cereal::make_nvp("vector", self.vertical()));
  }

  template <typename Archive>
  void load(Archive& archive, Vertical<Surface>& self) {
    std::shared_ptr<const Surface> surface;
    Vector<typename Surface::Coordinate> vertical;
    archive(cereal::make_nvp("surface", surface));
    archive(cereal::make_nvp("vector", vertical));
    self = Vertical(surface, vertical);
  }
};

}  // namespace flatsurf

#endif
