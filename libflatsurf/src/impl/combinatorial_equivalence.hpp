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

#include <optional>
#include <unordered_map>

#include "../../flatsurf/half_edge.hpp"
#include "../../flatsurf/equivalence.hpp"
#include "../../flatsurf/equivalence_class_code.hpp"

#ifndef LIBFLATSURF_COMBINATORIAL_EQUIVALENCE_HPP
#define LIBFLATSURF_COMBINATORIAL_EQUIVALENCE_HPP

namespace flatsurf {

template <typename Surface>
struct CombinatorialEquivalence : ImplementationOf<Equivalence<Surface>> {
  using Word = std::vector<int>;
  using Code = std::vector<Word>;
  using Predicate = std::function<bool(const Surface&, Edge)>;

  struct CombinatorialWalker {
    CombinatorialWalker(const Surface& surface, HalfEdge start, const Predicate& predicate);

    static std::vector<CombinatorialWalker> seedWalkers(const Surface&, const Predicate&);

    static void append(Code&, const Word&);

    static int cmp(const std::optional<Word>&, const std::optional<Word>&);

    std::optional<Word> walk();

    int label(const Surface& surface, const HalfEdge halfEdge);

    // Map half edges to 0, 1, 2, … as we encounter them when walking the
    // surface. By construction this will only contain a single signed half
    // edge for each edge.
    // (Note that we don't use a HalfEdge map since it is allocating a linear
    // amount of memory from the start which would mean that we'd get quadratic
    // runtime by just initializing linearly many runners.)
    std::unordered_map<HalfEdge, int> labels;

    // The set of labeled half edges in the order they were encountered.
    // Essentially, this is the inverse mapping of labels.
    std::vector<HalfEdge> labeled;

    int steps = 0;
    const Surface* surface;
    const Predicate* predicate;
  };

  struct CombinatorialEquivalenceClassCode : public EquivalenceClassCode {
    explicit CombinatorialEquivalenceClassCode(Code code);

    size_t hash() const override;
    bool equal(const EquivalenceClassCode&) const override;
    std::string toString() const override;

   private:
    Code code;
  };

  CombinatorialEquivalence(Predicate predicate);

  std::unique_ptr<EquivalenceClassCode> code(const Surface&) const override;
  Iterable<Deformation<Surface>> automorphisms() const override;
  void normalize(Surface&) const override;
  bool equal(const ImplementationOf<Equivalence<Surface>>&) const override;
  std::string toString() const override;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const CombinatorialEquivalence<S>&);

 private:
  Predicate predicate;
};

}

#endif
