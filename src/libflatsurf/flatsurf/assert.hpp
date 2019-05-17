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

#include <boost/preprocessor/stringize.hpp>

// This file has no header guards on purpose. It should not be include by other
// headers, just by implementation .cc files.

// Run a (cheap) check that a (user provided) argument is valid.
// If the check should be disabled when NDEBUG is defined, e.g., because it
// occurs in a hotspot, use ASSERT_ARGUMENT instead.
#define CHECK_ARGUMENT_(CONDITION)                                             \
  while (not(CONDITION)) {                                                     \
    throw std::invalid_argument(#CONDITION                                     \
                                " in " __FILE__ BOOST_PP_STRINGIZE(__LINE__)); \
  }
#define CHECK_ARGUMENT(CONDITION, MESSAGE)                                    \
  while (not(CONDITION)) {                                                    \
    throw std::invalid_argument(                                              \
        #CONDITION " " MESSAGE " in " __FILE__ BOOST_PP_STRINGIZE(__LINE__)); \
  }

#ifdef NDEBUG
#define ASSERT_ARGUMENT_(CONDITION) \
  while (false) {                   \
  }
#define ASSERT_ARGUMENT(CONDITION, MESSAGE) \
  while (false) {                           \
  }
#else
#define ASSERT_ARGUMENT_(CONDITION) CHECK_ARGUMENT_(CONDITION)
#define ASSERT_ARGUMENT(CONDITION, MESSAGE) CHECK_ARGUMENT(CONDITION, MESSAGE)
#endif
