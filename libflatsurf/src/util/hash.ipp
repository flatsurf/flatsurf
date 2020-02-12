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

#ifndef LIBFLATSURF_UTIL_HASH_IPP
#define LIBFLATSURF_UTIL_HASH_IPP

#include <cstddef>
#include <cstdint>
#include <functional>
#include <type_traits>

#include "false.ipp"

namespace flatsurf {

// Strangely boost's hash_combine is not very good
// (https://stackoverflow.com/questions/35985960/c-why-is-boosthash-combine-the-best-way-to-combine-hash-values)
// It's maybe not so bad in practice but pulling in the entire header just for
// this one function seems a bit too much.
// So we rolled our own hash_combine since strangely there's no header only for that yet.

template <typename T>
inline size_t hash_combine(const T& t) {
  return std::hash<T>()(t);
}

// TODO: Test this
template <typename S, typename T, typename... Args>
inline size_t hash_combine(const S& s, const T& t, const Args&... args) {
  if constexpr (std::is_same_v<size_t, uint64_t>) {
    // http://mostlymangling.blogspot.com/2019/01/better-stronger-mixer-and-test-procedure.html, public domain
    const auto rrxmxmx = [](size_t v) {
      const auto ror64 = [](size_t word, unsigned int shift) {
        return (word >> (shift & 63)) | (word << ((-shift) & 63));
      };

      v ^= ror64(v, 49) ^ ror64(v, 24);
      v *= 0x9FB21C651E98DF25L;
      v ^= v >> 28;
      v *= 0x9FB21C651E98DF25L;
      return v ^ v >> 28;
    };

    return hash_combine(rrxmxmx(std::hash<T>()(t)) ^ std::hash<S>()(s), args...);
  } else {
    static_assert(false_type_v<T>, "Hashing not implemented for your size_t yet");
  }
}

}  // namespace flatsurf

#endif
