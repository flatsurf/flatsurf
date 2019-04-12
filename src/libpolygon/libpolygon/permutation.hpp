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

#ifndef LIBPOLYGON_PERMUTATION_HPP
#define LIBPOLYGON_PERMUTATION_HPP

#include <iosfwd>
#include <vector>
#include <functional>

#include "libpolygon/libpolygon.hpp"

namespace flatsurf {
	// A type-safe permutation of items of type T.
	// There should be no runtime overhead to using a simple T[], at least when
	// compiled with -flto.
	template<typename T>
	struct Permutation {
		 explicit Permutation(const std::vector<std::vector<T>>& cycles);
		 explicit Permutation(const std::vector<std::pair<T, T>>& permutation);
		 T operator()(const T& t) const;
		 template<typename S>
		 static Permutation<T> create(const std::vector<std::vector<S>>&, const std::function<T(S)>&);

		 template<typename S>
     friend std::ostream& operator<<(std::ostream&, const Permutation<S>&);
		 size_t size() const;
		 size_t index(const T&) const;
		 const std::vector<T>& domain() const;
	 private:
		 std::vector<T> data;
	};
}

#endif

