/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2022-2025 Julian Rüth
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

#ifndef LIBFLATSURF_LINEAR_EQUIVALENCE_CLASS_CODE_HPP
#define LIBFLATSURF_LINEAR_EQUIVALENCE_CLASS_CODE_HPP

#include "equivalence_class_code.hpp"
#include "combinatorial_equivalence_class_code.hpp"

namespace flatsurf {

template <typename Surface>
class LinearEquivalenceClassCode : public EquivalenceClassCode {
 public:
  using T = typename Surface::Coordinate;
  using Word = std::tuple<CombinatorialEquivalenceClassCode::Word, std::vector<Vector<T>>>;

  explicit LinearEquivalenceClassCode(Word word);

  size_t hash() const override;
  bool equal(const EquivalenceClassCode&) const override;
  std::string toString() const override;

 private:
  Word word;
};

}

#endif

