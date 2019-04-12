/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2018 Alex Eskin
 *        Copyright (C) 2019 Vincent Delecroix
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
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

#include <list>

#include "./dir.h"

namespace polygon {
template <typename PointT>
class DMap  // developing map class
{
 public:
  explicit DMap(const Dir<PointT>& dir);
  Dir<PointT>& _start();  // this is the Dir we are following

  std::list<OEdge>::iterator&
  _next_edge();    // next face will be on the edge on the other side of this
  void advance();  // go to the next face (following DMap._start() )
  Face* current_face();  // the face containing the current edge
  std::list<OEdge>::iterator
  current_edge();  // the current edge cut by the line we are
                   // following. Always oriented to that the head of
                   // the edge is CCW from the line.

  PointT& _current_vert_pos();  // the position of the vertex at the head of
                                // current_edge, where base of start is (0,0)
                                // this is our candidate

  bool going_to_hit();  // are we about to hit a vertex along _start()?
  Dir<PointT>& _vertex_to_hit();  // if yes, this is the vertex we will hit

  Dir<PointT> current_vert_Dir();  // the Dir whose vec is -vert_pos(), oriented
                                   // from vert_pos towards the base of start;

  PointT& _get_cf_offset();  // will eventually remove this

 private:
  void setup();
  Dir<PointT> strt;
  std::list<OEdge>::iterator c_edge;
  PointT cf_offset;  // offset of current face, i.e c_edge.in_face();

  bool m_going_to_hit;
  Dir<PointT> m_vertex_to_hit;

  std::list<OEdge>::iterator m_next_edge;

  PointT m_current_vert_pos;
  Dir<PointT> m_current_vert_Dir;

  PointT m_next_candidate;  // internal use by setup() only
};
}  // namespace polygon
