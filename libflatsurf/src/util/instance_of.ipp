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

// Heavily inspired by https://stackoverflow.com/a/56818333/812379

namespace flatsurf {

template <typename T, template <typename...> typename Template>
struct is_instance_of : std::false_type {};

template <template <typename...> typename Template, typename... Args>
struct is_instance_of<Template<Args...>, Template> : std::true_type {};

template <typename T, template <typename...> typename Template>
constexpr bool is_instance_of_v = is_instance_of<T, Template>::value;

}  // namespace flatsurf
