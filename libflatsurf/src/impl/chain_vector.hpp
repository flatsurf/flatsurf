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

#ifndef LIBFLATSURF_CHAIN_VECTOR_IMPL_HPP
#define LIBFLATSURF_CHAIN_VECTOR_IMPL_HPP

#include <deque>
#include <optional>
#include <iosfwd>

#include "../../flatsurf/chain.hpp"

namespace flatsurf {

// A cached vector associated to a Chain. This equals the sum of the half edges
// that make up the chain; however, there are some optimizations to make this
// more efficient than just always keeping an updated vector around in the
// chain.
template <typename Surface, typename T>
class ChainVector {
 public:
  ChainVector(const ChainVector&) = delete;
  ChainVector(const ChainVector&&) = delete;
  ChainVector(const ImplementationOf<Chain<Surface>>*);
  ChainVector(const ImplementationOf<Chain<Surface>>*, Vector<T>);
  ChainVector(const ImplementationOf<Chain<Surface>>*, const ChainVector&);
  ChainVector(const ImplementationOf<Chain<Surface>>*, ChainVector&&);

  operator const Vector<T> &() const;

  void reset() const;

  ChainVector& operator+=(const ChainVector<Surface, T>&);
  ChainVector& operator+=(ChainVector<Surface, T>&&);

  ChainVector& operator-=(const ChainVector<Surface, T>&);
  ChainVector& operator-=(ChainVector<Surface, T>&&);

  ChainVector& operator+=(HalfEdge);
  ChainVector& operator-=(HalfEdge);

  ChainVector& operator=(const Vector<T>&) noexcept;
  ChainVector& operator=(Vector<T>&&) noexcept;

  ChainVector& operator=(const ChainVector&) = delete;
  ChainVector& operator=(ChainVector&&) = delete;

  template <typename S, typename TT>
  friend std::ostream& operator<<(std::ostream&, const ChainVector<S, TT>&);

 private:
  const ImplementationOf<Chain<Surface>>& chain;

  mutable std::optional<Vector<T>> value = std::nullopt;

  enum class MOVE {
    ADD,
    SUB
  };

  mutable double pendingMovesCost = 0;
  mutable std::deque<std::pair<MOVE, Vector<T>>> pendingMoves = {};

  template <typename S, typename TT>
  friend class ChainVector;

  double recomputeCost();

  template <typename V>
  ChainVector& record(MOVE, V&&);
};

template <typename Surface, typename T>
ChainVector(const ImplementationOf<Chain<Surface>>*, const ChainVector<Surface, T>&) -> ChainVector<Surface, T>;

}  // namespace flatsurf

#endif
