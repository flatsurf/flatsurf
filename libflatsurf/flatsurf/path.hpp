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

#ifndef LIBFLATSURF_PATH_HPP
#define LIBFLATSURF_PATH_HPP

#include <list>
#include <vector>

#include <boost/operators.hpp>

#include "copyable.hpp"
#include "serializable.hpp"

namespace flatsurf {

// TODO: Make sure this is used everywhere!
// A connected list of SaddleConnections in a Surface.
template <typename Surface>
class Path : public Serializable<Path<Surface>>,
             boost::equality_comparable<Path<Surface>> {
  using Segment = SaddleConnection<Surface>;
  using T = typename Surface::Coordinate;
 public:
  Path();
  Path(const std::vector<Segment>&);

  operator const std::vector<Segment>&() const;

  bool operator==(const Path&) const;

  // Return whether the list is cyclic, i.e., the last element joins up with the first.
  bool closed() const;
  // Return whether there are no segments showing up more than once.
  bool simple() const;
  // Return whether there are no segments followed by their negatives.
  bool reduced() const;

  Path reversed() const;

  T area() const;

  void splice(const PathIterator<Surface>&, Path& other);
  void splice(const PathIterator<Surface>&, Path&& other);

  void push_front(const Segment&);
  void push_back(const Segment&);

  size_t size() const;

  bool empty() const;

  PathIterator<Surface> begin() const;
  PathIterator<Surface> end() const;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const Path<S>&);

 private:
  using Implementation = ::flatsurf::Implementation<Path>;
  Copyable<Implementation> impl;

  friend Implementation;
  friend PathIterator<Surface>;
  friend ::flatsurf::Implementation<PathIterator<Surface>>;
};

}  // namespace flatsurf
#endif

