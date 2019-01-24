/**********************************************************************
 *  This file is part of Polygon.
 *
 *        Copyright (C) 2018 Alex Eskin
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

#include <boost/lexical_cast.hpp>
#include <cassert>
#include "libpolygon/number_field.h"
#include "libpolygon/params.h"
#include "libpolygon/two_complex.h"

namespace polygon {
bool is_alg_quotient(const alg_t<int64_t> &p1, const alg_t<int64_t> &p2,
                     algebraicQ &ratio)  // if p1/p2 is algebraic, return true
                                         // and put it in ratio.
{
  //    out_s << endl << "In is_alg_quotient, p1 = " << p1 << " p2 = " << p2 <<
  //    endl;

  int i;
  algebraicI q(p1.coeffs[0].field());
  //   algebraic<int64_t> zero_F = algebraic<int64_t>(p1.coeffs[0].field());

  for (i = 0; i <= Params::nbr_params(); i++) {
    q = p2.coeffs[i];
    if (q != NumberField<int64_t>::F_zero) {
      break;
    }
  }

  if (q == NumberField<int64_t>::F_zero) {
    ERR_RET("alg_t: division by 0");
  }

  algebraicQ num(to_rational(p1.coeffs[i]));
  algebraicQ denom(to_rational(p2.coeffs[i]));
  //    out_s << "num = " << num << " denom = " << denom << endl;

  ratio = num / denom;
  //  out_s << "Ratio is : " << ratio << flush;

  for (i = 0; i <= Params::nbr_params(); i++) {
    algebraicQ denom_i = to_rational(p2.coeffs[i]);
    algebraicQ num_i = to_rational(p1.coeffs[i]);

    if (ratio * denom_i != num_i) {
      return (false);
    }
  }
  //    out_s << "ratio = " << endl ;
  return (true);
}
}  // namespace polygon
