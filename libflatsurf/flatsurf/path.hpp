/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2021-2022 Julian Rüth
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
#include <optional>
#include <vector>

#include "copyable.hpp"
#include "serializable.hpp"

namespace flatsurf {

// A connected list of Segments in a Surface.
template <typename Surface>
class Path : public Serializable<Path<Surface>>,
             boost::equality_comparable<Path<Surface>> {
  using T = typename Surface::Coordinate;

 public:
  Path() noexcept;
  Path(const Segment<Surface>&);
  Path(const std::vector<Segment<Surface>>&);

  [[deprecated("crete a path from the .segment() of a saddle connection instead")]]
  Path(const SaddleConnection<Surface>&);
  [[deprecated("crete a path from the sequence of .segment() of a saddle connections instead")]]
  Path(const std::vector<SaddleConnection<Surface>>&);

  [[deprecated("a path cannot always be converted to a sequence of saddle connections; convert to a vector of segments instead")]]
  operator const std::vector<SaddleConnection<Surface>> &() const;

  operator const std::vector<Segment<Surface>> &() const;

  // Return whether two paths are indistinguishable, i.e., they are made up of
  // the same sequence of segments.
  bool operator==(const Path&) const;

  // Return whether the path is cyclic, i.e., the last element joins up with the first.
  bool closed() const;

  // Return whether there are no segments showing up more than once.
  // TODO: We need to consider partial segments here.
  bool simple() const;

  // Return whether there are no segments followed by their negatives.
  // TODO: We need to consider partial segments here.
  bool reduced() const;

  // Return whether the two paths are equivalent in homotopy.
  // The equivalence is relative to the start and end point, in particular this
  // function returns false if their respective start and end points to do not
  // match.
  bool homotopic(const Path&) const;

  // Return the shortest path that is homotopic to this path.
  Path tighten() const;

  Path reversed() const;

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

  // Return a single segment that is equivalent to this path.
  // Return nullopt if this path is not made up of segments that join up to a
  // single segment.
  std::optional<Segment<Surface>> segment() const;

  void push_front(const Segment<Surface>&);
  void push_back(const Segment<Surface>&);

  [[deprecated("push the saddle connection's .segment() instead")]]
  void push_front(const SaddleConnection<Surface>&);
  [[deprecated("push the saddle connection's .segment() instead")]]
  void push_back(const SaddleConnection<Surface>&);

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
  friend SegmentIterator<Surface>;
  friend ImplementationOf<iterator>;
  friend ImplementationOf<SegmentIterator<Surface>>;
};

template <typename Surface>
Path(const std::vector<SaddleConnection<Surface>>&) -> Path<Surface>;

template <typename Surface>
Path(const SaddleConnection<Surface>&) -> Path<Surface>;

}  // namespace flatsurf

#endif
