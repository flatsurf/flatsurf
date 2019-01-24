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
#include "libpolygon/two_complex.h"

int Params::n_params = 0;
vector<COORD> Params::params;

int Params::nbr_params() { return n_params; }

void Params::AddParams(int n, COORD prms[]) {
  for (int i = 0; i < n; i++) {
    params.push_back(prms[i]);
    //	out_s << "Param: t" << n_params+1+i<<" = "<< prms[i] << endl;
  }
  n_params += n;
}

void Params::print(ostream &out) {
  out << "#Params:" << nbr_params() << endl;
  for (int i = 0; i < nbr_params(); i++) {
    out << "#t" << i + 1 << "=";
    out << boost::lexical_cast<std::string>(params[i]) << endl;
  }
}

algebraic<bigrat> operator/(const algebraic<bigrat> &a, int n) {
  return a * bigrat(1, n);
}

algebraic<bigrat> operator*(const algebraic<bigrat> &r, int n) {
  return r * bigrat(n, 1);
}

algebraic<bigrat> operator*(int n, const algebraic<bigrat> &r) {
  return r * bigrat(n, 1);
}

alg_tI convert_to_algtI(alg_tQ p) {
  vector<algebraicI> new_coeffs;
  new_coeffs.clear();
  for (int i = 0; i <= Params::nbr_params(); i++) {
    vector<int64_t> new_coords;
    new_coords.clear();
    algebraicQ a = p.get_coeff(i);
    vector<bigrat> c = a.get_coords();

    for (unsigned int j = 0; j < c.size(); j++) {
      //`	    bigrat r = c[j];
      assert(c[j].get_den() == mpz_class(1));
      mpz_class tmp = c[j].get_num();
      if (!tmp.fits_slong_p()) {
        ERR_RET("cannot convert alg_tQ to alg_tI");
      }

      new_coords.push_back(tmp.get_si());
    }
    new_coeffs.push_back(algebraicI(new_coords, NumberField<int64_t>::F));
  }
  return (alg_tI(new_coeffs));
}

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
