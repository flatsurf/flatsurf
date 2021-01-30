/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2021 Julian Rüth
 *
 *  flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  flatsurf is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with flatsurf. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#ifndef LIBFLATSURF_UTIL_ASSERT_IPP
#define LIBFLATSURF_UTIL_ASSERT_IPP

#include <boost/preprocessor/stringize.hpp>
#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <sstream>

namespace flatsurf {
namespace {

// A throw statement that can be used in noexcept marked blocks without
// triggering compiler warnings.
template <typename E>
void throw_for_assert(const E& e) { throw e; }

// Return whether an environment variable should be considered as set.
bool isSet(const char* env) {
  const auto* ptr = std::getenv(env);
  if (ptr == nullptr) return false;

  std::string value = ptr;
  boost::trim(value);

  if (value == "0") return false;
  if (boost::iequals(value, "no")) return false;
  if (boost::iequals(value, "false")) return false;

  return true;
}

// Return whether all CHECK_ and LIBFLATSURF_ASSERT_ macros have been disabled at runtime
// through the LIBFLATSURF_NOCHECK environment variable.
bool nocheck() {
  static bool value = isSet("LIBFLATSURF_NOCHECK");
  return value;
}

// Return whether all LIBFLATSURF_ASSERT_ macros have been disabled at runtime through the
// LIBFLATSURF_NOASSERT environment variable.
bool noassert() {
  if (nocheck()) return true;

  static bool value = isSet("LIBFLATSURF_NOASSERT");
  return value;
}

}
} // namespace flatsurf

#define LIBFLATSURF_ASSERT_(CONDITION, EXCEPTION, MESSAGE)                      \
  while (BOOST_UNLIKELY(static_cast<bool>(not(CONDITION)))) {                   \
    (void)noassert;                                                             \
    std::stringstream user_message, assertion_message;                          \
    user_message << MESSAGE;                                                    \
    assertion_message << (#CONDITION " does not hold");                         \
    if (user_message.str().size())                                              \
      assertion_message << ": " << user_message.str();                          \
    else                                                                        \
      assertion_message << " ";                                                 \
    assertion_message << " in " __FILE__ ":" BOOST_PP_STRINGIZE(__LINE__);      \
    ::flatsurf::throw_for_assert(EXCEPTION(assertion_message.str().c_str())); \
  }

// Run a (cheap) check that a (user provided) argument is valid.
// If the check should be disabled when NDEBUG is defined, e.g., because it
// occurs in a hotspot, use LIBFLATSURF_ASSERT_ARGUMENT instead.
#define LIBFLATSURF_CHECK_ARGUMENT_(CONDITION) LIBFLATSURF_ASSERT_(::flatsurf::nocheck() || (CONDITION), std::invalid_argument, "")
#define LIBFLATSURF_CHECK_ARGUMENT(CONDITION, MESSAGE) LIBFLATSURF_ASSERT_(::flatsurf::nocheck() || (CONDITION), std::invalid_argument, MESSAGE)
#define LIBFLATSURF_CHECK(CONDITION, MESSAGE) LIBFLATSURF_ASSERT_(::flatsurf::nocheck() || (CONDITION), std::logic_error, MESSAGE)

#ifdef NDEBUG

#define LIBFLATSURF_ASSERT_CONDITION(CONDITION) (true || ::flatsurf::noassert() || (CONDITION))

#else

#define LIBFLATSURF_ASSERT_CONDITION(CONDITION) (::flatsurf::noassert() || (CONDITION))

#endif

#define LIBFLATSURF_ASSERT_ARGUMENT_(CONDITION) LIBFLATSURF_CHECK_ARGUMENT_(LIBFLATSURF_ASSERT_CONDITION(CONDITION))
#define LIBFLATSURF_ASSERT_ARGUMENT(CONDITION, MESSAGE) LIBFLATSURF_CHECK_ARGUMENT(LIBFLATSURF_ASSERT_CONDITION(CONDITION), MESSAGE)
#define LIBFLATSURF_ASSERT(CONDITION, MESSAGE) LIBFLATSURF_ASSERT_(LIBFLATSURF_ASSERT_CONDITION(CONDITION), std::logic_error, MESSAGE)
#define LIBFLATSURF_ASSERTIONS(LAMBDA) if (not LIBFLATSURF_ASSERT_CONDITION(false)) LAMBDA()

#define LIBFLATSURF_UNREACHABLE(MESSAGE)                  \
  {                                           \
    LIBFLATSURF_ASSERT_(false, std::logic_error, MESSAGE) \
    __builtin_unreachable();                  \
  }

#endif
