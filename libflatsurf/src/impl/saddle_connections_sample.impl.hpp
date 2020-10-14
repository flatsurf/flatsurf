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

#ifndef LIBFLATSURF_SADDLE_CONNECTIONS_SAMPLE_IMPL_HPP
#define LIBFLATSURF_SADDLE_CONNECTIONS_SAMPLE_IMPL_HPP

#include "../../flatsurf/saddle_connections_sample.hpp"
#include "saddle_connections.impl.hpp"

namespace flatsurf {

template <typename Surface>
class ImplementationOf<SaddleConnectionsSample<Surface>> : public ImplementationOf<SaddleConnections<Surface>> {
  using T = typename Surface::Coordinate;

 public:
  ImplementationOf(const ImplementationOf<SaddleConnections<Surface>>& connections);
};

template <typename Surface>
template <typename... Args>
SaddleConnectionsSample<Surface>::SaddleConnectionsSample(PrivateConstructor, Args&&... args) :
  self(spimpl::make_impl<ImplementationOf<SaddleConnectionsSample>>(std::forward<Args>(args)...)) {}

}  // namespace flatsurf

#endif

