/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020-2022 Julian Rüth
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

#include "../flatsurf/bound.hpp"
#include "cereal.helpers.hpp"

namespace flatsurf::test {

TEST_CASE("Bound Creation", "[Bound][constructor]") {
  REQUIRE(Bound(1) == Bound((char)1));
  REQUIRE(Bound(1) == Bound((unsigned char)1));
  REQUIRE(Bound(1) == Bound((short)1));
  REQUIRE(Bound(1) == Bound((unsigned short)1));
  REQUIRE(Bound(1) == Bound((int)1));
  REQUIRE(Bound(1) == Bound((unsigned int)1));
  REQUIRE(Bound(1) == Bound((long)1));
  REQUIRE(Bound(1) == Bound((unsigned long)1));
  REQUIRE(Bound(1) == Bound((long long)1));
  REQUIRE(Bound(1) == Bound((unsigned long long)1));
  REQUIRE(Bound(1) == Bound(mpz_class(1)));
  REQUIRE(Bound(1, 1) == Bound((char)1, (char)1));
  REQUIRE(Bound(1, 1) == Bound((char)1, (unsigned char)1));
  REQUIRE(Bound(1, 1) == Bound((char)1, (short)1));
  REQUIRE(Bound(1, 1) == Bound((char)1, (unsigned short)1));
  REQUIRE(Bound(1, 1) == Bound((char)1, (int)1));
  REQUIRE(Bound(1, 1) == Bound((char)1, (unsigned int)1));
  REQUIRE(Bound(1, 1) == Bound((char)1, (long)1));
  REQUIRE(Bound(1, 1) == Bound((char)1, (unsigned long)1));
  REQUIRE(Bound(1, 1) == Bound((char)1, (long long)1));
  REQUIRE(Bound(1, 1) == Bound((char)1, (unsigned long long)1));
  REQUIRE(Bound(1, 1) == Bound((char)1, mpz_class(1)));
  REQUIRE(Bound(1, 1) == Bound((unsigned char)1, (char)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned char)1, (unsigned char)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned char)1, (short)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned char)1, (unsigned short)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned char)1, (int)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned char)1, (unsigned int)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned char)1, (long)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned char)1, (unsigned long)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned char)1, (long long)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned char)1, (unsigned long long)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned char)1, mpz_class(1)));
  REQUIRE(Bound(1, 1) == Bound((short)1, (char)1));
  REQUIRE(Bound(1, 1) == Bound((short)1, (unsigned char)1));
  REQUIRE(Bound(1, 1) == Bound((short)1, (short)1));
  REQUIRE(Bound(1, 1) == Bound((short)1, (unsigned short)1));
  REQUIRE(Bound(1, 1) == Bound((short)1, (int)1));
  REQUIRE(Bound(1, 1) == Bound((short)1, (unsigned int)1));
  REQUIRE(Bound(1, 1) == Bound((short)1, (long)1));
  REQUIRE(Bound(1, 1) == Bound((short)1, (unsigned long)1));
  REQUIRE(Bound(1, 1) == Bound((short)1, (long long)1));
  REQUIRE(Bound(1, 1) == Bound((short)1, (unsigned long long)1));
  REQUIRE(Bound(1, 1) == Bound((short)1, mpz_class(1)));
  REQUIRE(Bound(1, 1) == Bound((unsigned short)1, (char)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned short)1, (unsigned char)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned short)1, (short)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned short)1, (unsigned short)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned short)1, (int)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned short)1, (unsigned int)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned short)1, (long)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned short)1, (unsigned long)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned short)1, (long long)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned short)1, (unsigned long long)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned short)1, mpz_class(1)));
  REQUIRE(Bound(1, 1) == Bound((int)1, (char)1));
  REQUIRE(Bound(1, 1) == Bound((int)1, (unsigned char)1));
  REQUIRE(Bound(1, 1) == Bound((int)1, (short)1));
  REQUIRE(Bound(1, 1) == Bound((int)1, (unsigned short)1));
  REQUIRE(Bound(1, 1) == Bound((int)1, (int)1));
  REQUIRE(Bound(1, 1) == Bound((int)1, (unsigned int)1));
  REQUIRE(Bound(1, 1) == Bound((int)1, (long)1));
  REQUIRE(Bound(1, 1) == Bound((int)1, (unsigned long)1));
  REQUIRE(Bound(1, 1) == Bound((int)1, (long long)1));
  REQUIRE(Bound(1, 1) == Bound((int)1, (unsigned long long)1));
  REQUIRE(Bound(1, 1) == Bound((int)1, mpz_class(1)));
  REQUIRE(Bound(1, 1) == Bound((unsigned int)1, (char)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned int)1, (unsigned char)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned int)1, (short)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned int)1, (unsigned short)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned int)1, (int)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned int)1, (unsigned int)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned int)1, (long)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned int)1, (unsigned long)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned int)1, (long long)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned int)1, (unsigned long long)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned int)1, mpz_class(1)));
  REQUIRE(Bound(1, 1) == Bound((long)1, (char)1));
  REQUIRE(Bound(1, 1) == Bound((long)1, (unsigned char)1));
  REQUIRE(Bound(1, 1) == Bound((long)1, (short)1));
  REQUIRE(Bound(1, 1) == Bound((long)1, (unsigned short)1));
  REQUIRE(Bound(1, 1) == Bound((long)1, (int)1));
  REQUIRE(Bound(1, 1) == Bound((long)1, (unsigned int)1));
  REQUIRE(Bound(1, 1) == Bound((long)1, (long)1));
  REQUIRE(Bound(1, 1) == Bound((long)1, (unsigned long)1));
  REQUIRE(Bound(1, 1) == Bound((long)1, (long long)1));
  REQUIRE(Bound(1, 1) == Bound((long)1, (unsigned long long)1));
  REQUIRE(Bound(1, 1) == Bound((long)1, mpz_class(1)));
  REQUIRE(Bound(1, 1) == Bound((unsigned long)1, (char)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned long)1, (unsigned char)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned long)1, (short)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned long)1, (unsigned short)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned long)1, (int)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned long)1, (unsigned int)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned long)1, (long)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned long)1, (unsigned long)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned long)1, (long long)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned long)1, (unsigned long long)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned long)1, mpz_class(1)));
  REQUIRE(Bound(1, 1) == Bound((long long)1, (char)1));
  REQUIRE(Bound(1, 1) == Bound((long long)1, (unsigned char)1));
  REQUIRE(Bound(1, 1) == Bound((long long)1, (short)1));
  REQUIRE(Bound(1, 1) == Bound((long long)1, (unsigned short)1));
  REQUIRE(Bound(1, 1) == Bound((long long)1, (int)1));
  REQUIRE(Bound(1, 1) == Bound((long long)1, (unsigned int)1));
  REQUIRE(Bound(1, 1) == Bound((long long)1, (long)1));
  REQUIRE(Bound(1, 1) == Bound((long long)1, (unsigned long)1));
  REQUIRE(Bound(1, 1) == Bound((long long)1, (long long)1));
  REQUIRE(Bound(1, 1) == Bound((long long)1, (unsigned long long)1));
  REQUIRE(Bound(1, 1) == Bound((long long)1, mpz_class(1)));
  REQUIRE(Bound(1, 1) == Bound((unsigned long long)1, (char)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned long long)1, (unsigned char)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned long long)1, (short)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned long long)1, (unsigned short)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned long long)1, (int)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned long long)1, (unsigned int)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned long long)1, (long)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned long long)1, (unsigned long)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned long long)1, (long long)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned long long)1, (unsigned long long)1));
  REQUIRE(Bound(1, 1) == Bound((unsigned long long)1, mpz_class(1)));
}

TEST_CASE("Bound Arithmetic", "[Bound][constructor][operator<][operator>][operator<=][operator>=][operator==]") {
  REQUIRE(Bound() < Bound(1));

  SECTION("Multiplication works as expected") {
    REQUIRE(Bound(1) < 2 * Bound(1));
    REQUIRE(Bound(2) == 2 * Bound(1));
  }

  SECTION("Bounds in Two Variables Compare Like Vector Lengths") {
    REQUIRE(Bound(1, 0) <= Bound(1));
    REQUIRE(Bound(1, 1) <= Bound(2));
    REQUIRE(Bound(2, 2) > Bound(2));
    REQUIRE(Bound(3, 4) == Bound(5));
  }

  SECTION("Non-zero Checks") {
    REQUIRE(static_cast<bool>(Bound()) == false);
    REQUIRE(static_cast<bool>(Bound(1)) == true);
  }
}

TEST_CASE("Serialization of a Bound", "[Bound][save][load]") {
  testRoundtrip(Bound(13, 37));
}

}  // namespace flatsurf::test
