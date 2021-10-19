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

#include "impl/chain_vector.hpp"

#include <ostream>
#include <optional>

#include "impl/chain.impl.hpp"
#include "util/assert.ipp"

namespace flatsurf {

namespace {

// These are just rough estimates. If Chain shows up a lot in the profiler, we
// might want to benchmark these to get better values here. (Or rather
// autotune them.)
template <typename T>
struct Cost {
  // The cost of a Vector<T> addition.
  static constexpr double add() {
    if (std::is_same_v<T, long long>)
      return 1;
    else if (std::is_same_v<T, exactreal::Arb>)
      return 2;
    else
      return 16;
  }

  // The cost of a Vector<T> copy.
  static constexpr double copy() {
    if (std::is_same_v<T, long long>) {
      return 1;
    } else if (std::is_same_v<T, mpz_class> || std::is_same_v<T, exactreal::Arb>) {
      return 2;
    } else if (std::is_same_v<T, mpq_class> || std::is_same_v<T, eantic::renf_elem_class>) {
      return 4;
    } else {
      return 8;
    }
  }

  // The cost of converting a Vector<T> to a Vector<Arb>.
  static constexpr double convert() {
    if (std::is_same_v<T, exactreal::Arb> || std::is_same_v<T, long long>) {
      return 1;
    } else if (std::is_same_v<T, mpz_class>) {
      return 2;
    } else {
      return 16;
    }
  }

  // The cost of a Vector<T> move.
  static constexpr double move() {
    return 1;
  }

  // The cost of a c * Vector<T> addition.
  static constexpr double addmul() {
    return 2 * add();
  }

  // The cost of recomputing a vector from the mpz coefficients from scratch.
  static constexpr double recompute(size_t edges) {
    return static_cast<double>(edges) * addmul();
  }
};

}  // namespace

template <typename Surface, typename T>
ChainVector<Surface, T>::ChainVector(const ImplementationOf<Chain<Surface>>* chain) :
  chain(*chain) {
}

template <typename Surface, typename T>
ChainVector<Surface, T>::ChainVector(const ImplementationOf<Chain<Surface>>* chain, Vector<T> value) :
  chain(*chain),
  value(std::move(value)) {
}

template <typename Surface, typename T>
ChainVector<Surface, T>::ChainVector(const ImplementationOf<Chain<Surface>>* chain, const ChainVector& value) :
  ChainVector(chain, static_cast<const Vector<T>&>(value)) {}

template <typename Surface, typename T>
ChainVector<Surface, T>& ChainVector<Surface, T>::operator=(const Vector<T>& value) noexcept {
  this->value = value;
  pendingMoves.clear();
  pendingMovesCost = 0;
  return *this;
}

template <typename Surface, typename T>
ChainVector<Surface, T>& ChainVector<Surface, T>::operator=(Vector<T>&& value) noexcept {
  this->value = std::move(value);
  pendingMoves.clear();
  pendingMovesCost = 0;
  return *this;
}

template <typename Surface, typename T>
ChainVector<Surface, T>& ChainVector<Surface, T>::operator+=(HalfEdge halfEdge) {
  (void)halfEdge;
  if (value) {
    const double storeAsPendingCost = pendingMovesCost + Cost<T>::copy() + Cost<T>::add();
    if (storeAsPendingCost > recomputeCost()) {
      value = std::nullopt;
      pendingMovesCost = 0;
      pendingMoves.clear();
    } else {
      if constexpr (std::is_same_v<T, exactreal::Arb>) {
        pendingMoves.emplace_back(MOVE::ADD, chain.surface->fromHalfEdgeApproximate(halfEdge));
      } else {
        pendingMoves.emplace_back(MOVE::ADD, chain.surface->fromHalfEdge(halfEdge));
      }
      pendingMovesCost += Cost<T>::add();
    }
  }

  return *this;
}

template <typename Surface, typename T>
ChainVector<Surface, T>& ChainVector<Surface, T>::operator-=(HalfEdge halfEdge) {
  return *this += -halfEdge;
}

template <typename Surface, typename T>
ChainVector<Surface, T>& ChainVector<Surface, T>::operator+=(const ChainVector<Surface, T>& rhs) {
  return record(MOVE::ADD, rhs);
}

template <typename Surface, typename T>
ChainVector<Surface, T>& ChainVector<Surface, T>::operator+=(ChainVector<Surface, T>&& rhs) {
  return record(MOVE::ADD, std::move(rhs));
}

template <typename Surface, typename T>
ChainVector<Surface, T>& ChainVector<Surface, T>::operator-=(const ChainVector<Surface, T>& rhs) {
  return record(MOVE::SUB, rhs);
}

template <typename Surface, typename T>
ChainVector<Surface, T>& ChainVector<Surface, T>::operator-=(ChainVector<Surface, T>&& rhs) {
  return record(MOVE::SUB, std::move(rhs));
}

template <typename Surface, typename T>
template <typename V>
ChainVector<Surface, T>& ChainVector<Surface, T>::record(MOVE move, V&& rhs) {
  if (value) {
    if (rhs.value) {
      // Both operands have a valid Vector<T>. We now have to decide whether
      // it's worth it to add rhs's vector to our pendingMoves list or whether
      // it's cheaper to recompute things from scratch because pendingMoves is
      // too long already.
      {
        const double storeAsPendingCost = rhs.pendingMovesCost + pendingMovesCost + (std::is_rvalue_reference_v<V> ? Cost<T>::move() : Cost<T>::copy()) + Cost<T>::add();

        if (storeAsPendingCost > recomputeCost()) {
          value = std::nullopt;
          pendingMovesCost = 0;
          pendingMoves.clear();
        } else {
          (void)static_cast<const Vector<T>&>(rhs);

          if constexpr (std::is_rvalue_reference_v<V>) {
            pendingMoves.emplace_back(move, std::move(*rhs.value));
          } else {
            pendingMoves.emplace_back(move, *rhs.value);
          }
          pendingMovesCost += Cost<T>::add();
        }
      }
    } else {
      value = std::nullopt;
      pendingMovesCost = 0;
      pendingMoves.clear();
    }
  }

  return *this;
}

template <typename Surface, typename T>
double ChainVector<Surface, T>::recomputeCost() {
  return std::is_same_v<T, exactreal::Arb> ?
                                           // The Arb representation is computed from the exact representation.
             (chain.vector.value ? (chain.vector.pendingMovesCost + Cost<typename Surface::Coordinate>::convert()) : (Cost<typename Surface::Coordinate>::recompute(chain.surface->size()) + Cost<typename Surface::Coordinate>::convert()))
                                           : (chain.vector.value ? chain.vector.pendingMovesCost : Cost<T>::recompute(chain.surface->size()));
}

template <typename Surface, typename T>
ChainVector<Surface, T>::operator const Vector<T> &() const {
  if (value) {
    for (const auto& [move, v] : pendingMoves) {
      switch (move) {
        case MOVE::ADD:
          *value += std::move(v);
          break;
        case MOVE::SUB:
          *value -= std::move(v);
          break;
        default:
          throw std::logic_error("unsupported MOVE type");
      }
    }
    pendingMoves.clear();
    pendingMovesCost = 0;
  } else {
    LIBFLATSURF_ASSERT(pendingMoves.empty(), "value and pendingMoves are out of sync");
    LIBFLATSURF_ASSERT(pendingMovesCost == 0, "value and pendingMovesCost are out of sync");

    using Coordinate = typename Surface::Coordinate;

    if constexpr (std::is_same_v<T, exactreal::Arb>) {
      const Vector<Coordinate>& exact = static_cast<const Vector<Coordinate>&>(chain);
      if (!value)
        value.emplace(exact);
    } else {
      static_assert(std::is_same_v<T, Coordinate>);

      Vector<T> exact;

      for (const Edge edge : chain.surface->edges()) {
        const auto coefficient = chain[edge.index()];
        if (coefficient)
          exact += **coefficient * static_cast<const Vector<T>&>(chain.surface->fromHalfEdge(edge.positive()));
      }

      value.emplace(std::move(exact));

      chain.approximateVector = static_cast<Vector<exactreal::Arb>>(*value);
    }
  }

  return *value;
}

template <typename Surface, typename T>
std::ostream& operator<<(std::ostream& os, const ChainVector<Surface, T>& self) {
  return os << static_cast<const Vector<T>&>(self);
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "../flatsurf/vector.hpp"
#include "util/instantiate.ipp"

#define LIBFLATSURF_INSTANTIATE_THIS(SURFACE)                                                          \
  LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION((ChainVector<SURFACE, typename SURFACE::Coordinate>)) \
  LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION((ChainVector<SURFACE, exactreal::Arb>))

LIBFLATSURF_INSTANTIATE_MANY((LIBFLATSURF_INSTANTIATE_THIS), LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
