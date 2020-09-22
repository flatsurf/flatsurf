/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Julian Rüth
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

#ifndef LIBFLATSURF_DECOMPOSITION_STEP_HPP
#define LIBFLATSURF_DECOMPOSITION_STEP_HPP

#include <optional>
#include <utility>

#include "forward.hpp"

namespace flatsurf {
template <typename Surface>
class DecompositionStep {
  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

  enum class Result {
    LIMIT_REACHED,
    CYLINDER,
    SEPARATING_CONNECTION,
    NON_SEPARATING_CONNECTION,
    WITHOUT_PERIODIC_TRAJECTORY,
    KEANE,
  };

  Result result;
  std::optional<FlowConnection<Surface>> connection = std::nullopt;
  std::optional<FlowComponent<Surface>> additionalComponent = std::nullopt;
};
}  // namespace flatsurf

#endif
