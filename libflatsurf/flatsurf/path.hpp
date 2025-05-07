/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2021 Julian Rüth
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

#include <boost/operators.hpp>
#include <iosfwd>
#include <vector>

#include "copyable.hpp"
#include "serializable.hpp"

namespace flatsurf {

// A connected list of SaddleConnections in a Surface.
template <typename Surface>
class Path : public Serializable<Path<Surface>>,
             boost::equality_comparable<Path<Surface>> {
  using Segment = SaddleConnection<Surface>;
  using T = typename Surface::Coordinate;

 public:
  Path() noexcept;
  Path(const Segment&);
  Path(const std::vector<Segment>&);

  operator const std::vector<Segment> &() const;

  bool operator==(const Path&) const;

  // Return whether the path is cyclic, i.e., the last element joins up with the first.
  bool closed() const;

  // Return whether there are no segments showing up more than once.
  bool simple() const;

  // Return whether there are no segments followed by their negatives.
  bool reduced() const;

  // Return whether the two paths are equivalent in homotopy.
  // The equivalence is relative to the start and end point, in particular this
  // function returns false if their respective start and end points to do not
  // match.
  bool homotopic(const Path&) const;

  // Return the shortest path that is homotopic to this path.
  Path tighten() const;

  Path reversed() const;

  [[deprecated("This method returns twice the signed area if this path describes a polygon in some obvious way. However, often this notion is not terribly meaningful so we are going to remove this method in a future release.")]]
  T area() const;

  void splice(const PathIterator<Surface>&, Path& other);
  void splice(const PathIterator<Surface>&, Path&& other);

  // Append `other` to this path.
  Path& operator+=(const Path& other);

  // Return the path that results from appending `other` to this path.
  Path operator+(const Path& other) const;

  // Return the inverse path, i.e., the path obtained by walking the reversed
  // connections in reverse order.
  Path operator-() const;

  void push_front(const Segment&);
  void push_back(const Segment&);

  void pop_front();
  void pop_back();

  size_t size() const;

  bool empty() const;

  using iterator = PathIterator<Surface>;

  iterator begin() const;
  iterator end() const;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const Path<S>&);

 private:
  Copyable<Path> self;

  friend ImplementationOf<Path>;
  friend iterator;
  friend ImplementationOf<iterator>;
};

template <typename Surface>
Path(const std::vector<SaddleConnection<Surface>>&) -> Path<Surface>;

template <typename Surface>
Path(const SaddleConnection<Surface>&) -> Path<Surface>;

}  // namespace flatsurf
#endif
