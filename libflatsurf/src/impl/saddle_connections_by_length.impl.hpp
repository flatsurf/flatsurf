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

#ifndef LIBFLATSURF_SADDLE_CONNECTIONS_BY_LENGTH_IMPL_HPP
#define LIBFLATSURF_SADDLE_CONNECTIONS_BY_LENGTH_IMPL_HPP

#include "../../flatsurf/saddle_connections_by_length.hpp"
#include "saddle_connections.impl.hpp"

namespace flatsurf {

template <typename Surface>
class ImplementationOf<SaddleConnectionsByLength<Surface>> : public ImplementationOf<SaddleConnections<Surface>> {
  using T = typename Surface::Coordinate;

 public:
  ImplementationOf(const ImplementationOf<SaddleConnections<Surface>>& connections) :
    ImplementationOf<SaddleConnections<Surface>>(connections) {}
};

// TODO: Use this pattern everywhere: Put the PrivateConstructor{} constructor into the impl file.
template <typename Surface>
template <typename... Args>
SaddleConnectionsByLength<Surface>::SaddleConnectionsByLength(PrivateConstructor, Args&&... args) :
  impl(spimpl::make_impl<Implementation>(std::forward<Args>(args)...)) {}

}  // namespace flatsurf

#endif
