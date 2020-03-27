/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Julian Rüth
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

#ifndef LIBFLATSURF_IMPL_LENGTHS_HPP
#define LIBFLATSURF_IMPL_LENGTHS_HPP

#include <gmpxx.h>
#include <deque>
#include <iosfwd>
#include <memory>
#include <vector>

#include <intervalxt/label.hpp>
#include <intervalxt/lengths.hpp>

#include "../../flatsurf/edge.hpp"
#include "../../flatsurf/edge_map.hpp"
#include "../../flatsurf/saddle_connection.hpp"

#include "flow_decomposition_state.hpp"

namespace flatsurf {

template <typename Surface>
class Lengths {
  using T = typename Surface::Coordinate;

 public:
  Lengths(std::shared_ptr<const Vertical<FlatTriangulation<T>>>, EdgeMap<std::optional<SaddleConnection<FlatTriangulation<T>>>>&&);

  void push(intervalxt::Label);
  void pop();
  void subtract(intervalxt::Label);
  intervalxt::Label subtractRepeated(intervalxt::Label);
  void subtractRepeated(intervalxt::Label, const mpz_class&);
  std::vector<mpq_class> coefficients(intervalxt::Label) const;
  int cmp(intervalxt::Label) const;
  int cmp(intervalxt::Label, intervalxt::Label) const;
  T get(intervalxt::Label) const;
  std::string render(intervalxt::Label) const;
  ::intervalxt::Lengths forget() const;
  ::intervalxt::Lengths only(const std::unordered_set<::intervalxt::Label>&) const;
  bool similar(::intervalxt::Label, ::intervalxt::Label, const ::intervalxt::Lengths&, ::intervalxt::Label, ::intervalxt::Label) const;

  // This is a hack, see https://github.com/flatsurf/flatsurf/issues/152.
  // Maybe the entire flatsurf::IntervalExchangeTransformation is a bit unfortunate actually.
  void registerDecomposition(std::shared_ptr<FlowDecompositionState<FlatTriangulation<T>>>);

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const Lengths<S>&);

 private:
  intervalxt::Label toLabel(Edge) const;
  Edge fromLabel(intervalxt::Label) const;

  T length(intervalxt::Label) const;
  T length() const;
  // Return whether this label is the leftmost label on a top contour.
  bool minuendOnTop(intervalxt::Label) const;
  // Return the component that holds this label.
  FlowComponentState<FlatTriangulation<T>>& component(intervalxt::Label) const;

  std::weak_ptr<FlowDecompositionState<FlatTriangulation<T>>> state;
  std::shared_ptr<const Vertical<FlatTriangulation<T>>> vertical;
  EdgeMap<std::optional<SaddleConnection<FlatTriangulation<T>>>> lengths;

  std::deque<intervalxt::Label> stack;
  T sum;

  size_t degree;

  friend IntervalExchangeTransformation<Surface>;
};

}  // namespace flatsurf

#endif
