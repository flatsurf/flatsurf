/**********************************************************************
 *  This file is part of Polygon.
 *
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Polygon is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Polygon is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Polygon. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#ifndef LIBPOLYGON_TWOCOMPLEX_CHILD_H
#define LIBPOLYGON_TWOCOMPLEX_CHILD_H

#include <stdexcept>

namespace polygon {
class TwoComplex;
// A type that is created for a TwoComplex and holds a reference back to its
// parent.
class TwoComplexChild {
 public:
  explicit TwoComplexChild(const TwoComplex& complex) : complex(complex) {}
  TwoComplexChild& operator=(const TwoComplexChild& child) {
    if (&child.complex != &this->complex) {
      throw new std::logic_error(
          "assignment must be between children of the same complex");
    }
    return *this;
  }
  const TwoComplex& complex;
};
}  // namespace polygon

#endif
