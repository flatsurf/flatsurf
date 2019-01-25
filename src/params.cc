/**********************************************************************
 *  This file is part of Polygon.
 *
 *        Copyright (C) 2018 Alex Eskin
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

#include <boost/lexical_cast.hpp>
#include <ostream>
#include <vector>

#include "libpolygon/params.h"

using std::endl;
using std::ostream;
using std::vector;

namespace polygon {
int Params::n_params = 0;
vector<COORD> Params::params;

int Params::nbr_params() { return n_params; }

void Params::AddParams(int n, COORD prms[]) {
  for (int i = 0; i < n; i++) {
    params.push_back(prms[i]);
  }
  n_params += n;
}

void Params::print(ostream &out) {
  out << "#Params:" << nbr_params() << endl;
  for (int i = 0; i < nbr_params(); i++) {
    out << "#t" << i + 1 << "=";
    out << boost::lexical_cast<std::string>(params[i]) << endl;
  }
}
}  // namespace polygon
