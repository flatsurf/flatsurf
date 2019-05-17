/**********************************************************************
 *  This file is part of flatsurf.
 *
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

#ifndef LIBFLATSURF_AS_VECTOR_HPP
#define LIBFLATSURF_AS_VECTOR_HPP

#include <vector>
#include <boost/range/iterator_range.hpp>
#include <boost/iterator/iterator_traits.hpp>

namespace {
  // Since BoostToOven::as_container never made it into boost, we use this little
  // helper to convert a boost range to a vector.
  template <typename Iterator>
  auto as_vector(const boost::iterator_range<Iterator>& range) {
    using T = typename boost::iterators::iterator_value<Iterator>::type;
    return std::vector<T>(range.begin(), range.end());
  }
}

#endif
