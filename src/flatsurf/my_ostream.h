/**********************************************************************
 *  This file is part of Flatsurf.
 *
 *        Copyright (C) 2018 Alex Eskin
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
 *  along with Flatsurf. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#ifndef FLATSURF_MY_OSTREAM
#define FLATSURF_MY_OSTREAM

#include <fstream>
#include <string>

namespace polygon {
class my_ostream {
 public:
  explicit my_ostream(const std::string& filename);
  std::ofstream& tri();
  std::ofstream& tex();
  void close();

 private:
  std::ofstream tri_stream;
  std::ofstream tex_stream;
};
}  // namespace polygon

#endif  // FLATSURF_MY_OSTREAM
