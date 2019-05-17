/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2018 Alex Eskin
 *        Copyright (C) 2019 Vincent Delecroix
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

#include <NTL/mat_ZZ.h>
#include <boost/algorithm/string/replace.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/math/special_functions/binomial.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/irange.hpp>
#include <complex>
#include <ostream>
#include <vector>

#include "./algebraic.h"
#include "./defs.h"
#include "./globals.h"
#include "./number_field.h"
#include "./params.h"
#include "./two_complex.h"
#include "flatsurf/detail/as_vector.hpp"
#include "flatsurf/vector_arb.hpp"

using boost::irange;
using boost::lexical_cast;
using boost::numeric_cast;
using boost::adaptors::transformed;
using boost::math::binomial_coefficient;
using exactreal::Element;
using exactreal::NumberFieldTraits;
using flatsurf::VectorExactReal;
using NTL::Mat;
using NTL::Vec;
using NTL::ZZ;
using std::complex;
using std::ostream;
using std::vector;

namespace polygon {
template <typename T>
poly<T> cyclotomic_poly(int n) {
  vector<T> coeffs;
  poly<T> current, quotient, remainder;

  coeffs.push_back(-1);
  for (int i = 1; i < n; i++) {
    coeffs.push_back(0);
  }
  coeffs.push_back(1);

  current = poly<T>(coeffs);

  if (n == 1) {
    return (current);
  }

  for (int d = 1; d < n; d++) {
    if (n % d == 0) {
      poly<T> g = cyclotomic_poly<T>(d);
      divide<T>(quotient, remainder, current, g);
      if (not(remainder == poly<T>())) {
        std::cout << "cyclotomic: failed to divide";
      }
      current = quotient;
    }
  }
  return (current);
}

/*
namespace {
        template<typename T>
        double to_double(const T& t){
                return static_cast<double>(t);
        }

        template<>
        double to_double<mpq_class>(const mpq_class& q){
                return q.get_d();
        }
}

// Return the minimal polynomial of 2*Re(ξ) where ξ is a primitive n-th root of
// unity; a totally real polynomial.
template <typename T>
poly<T> real_cyclotomic_poly(int n) {
        auto cyclotomic = cyclotomic_poly<T>(n);
        auto d = numeric_cast<long>(cyclotomic.degree());
        assert(d%2 == 0);
        d /= 2;

        // Since the cyclotomic polynomials are reciprocal/palindromic, we can
write them as F(x) = x^d·f(x+1/x).
        // (ξ + 1/ξ = 2*Re(ξ) is a root of f.)
        // We compute f by comparing coefficients and solving the resulting
system of linear equations. auto A = Mat<ZZ>(); A.SetDims(d + 1, d + 1); auto b
= Vec<ZZ>(); b.SetLength(d + 1); for (long i = 0; i <= d; i++) { for (long j =
0; j <= i/2; j++) { auto bin =
numeric_cast<long>(binomial_coefficient<double>(numeric_cast<unsigned>(i),
numeric_cast<unsigned>(j))); A(1 + i, 1 + i - 2*j) += bin;
                }
                b(1 + i) =
numeric_cast<long>(to_double(cyclotomic.coefficients[numeric_cast<size_t>(d -
i)]));
        }

        ZZ det;
        Vec<ZZ> x;
        solve(det, x, A, b);
        assert(det == 1);

        return poly<T>(from(x).select([](const ZZ& c){ return
T(NTL::conv<long>(c)); }).toVector());
}
*/

// Return the smallest totally real field which contains Re(ξ) and Im(ξ) where
// ξ is a primitive n-th root of unity; together with the elements Re(ξ) and
// Im(ξ).
std::tuple<std::shared_ptr<eantic::renf_class>, eantic::renf_elem_class,
           eantic::renf_elem_class>
real_cyclotomic_field(size_t n) {
  /*
   Currently, we use hard coded minimal polynomials of these number fields.
   They were generated in Sage with the following code:

def choose_roots(n, emb, re_roots, im_roots):
  for re in re_roots:
          if n > 3 and emb(re) < 0: continue
          for im in im_roots:
                  if emb(im) < 0: continue
                  err = abs(emb(re)**2 + emb(im)**2 - 1)
                  if err < 1e-3:
                          return re, im
  assert(False)

for n in range(32):
  if n == 0: n = 1
  R.<x> = QQ[]
  K.<a> = CyclotomicField(n)
  imaginary = (x^2 + 1).change_ring(K)
  if imaginary.is_irreducible():
          K.<i> = K.extension(x^2 + 1)
  else:
          i = imaginary.roots()[0][0]
  re = (a + ~a)/2
  re = re.absolute_minpoly()
  im = -i*(a - ~a)/2
  im = im.absolute_minpoly()

  K.<a> = NumberField(re)

  L.<b> = K.extension(sorted([f for f,e in im.change_ring(K).factor()], key =
lambda f: f.degree())[0])

  M.<x> = L.absolute_field()

  emb = M.embeddings(RR)[0]

  re, im = choose_roots(n, emb, re.change_ring(M).roots(multiplicities=False),
im.change_ring(M).roots(multiplicities=False))

  print('{"%s", "%s", "%s", "%s"},'%(M.defining_polynomial(), emb(x), re, im))

*/
  vector<vector<std::string>> totally_real_data(
      {{"x", "0.000000000000000", "1", "0"},
       {"x", "0.000000000000000", "1", "0"},
       {"x", "0.000000000000000", "-1", "0"},
       {"x^2 - 3", "-1.73205080756888", "-1/2", "-1/2*x"},
       {"x - 1", "1.00000000000000", "0", "1"},
       {"x^4 - 5*x^2 + 5", "-1.90211303259031", "1/2*x^2 - 3/2", "-1/2*x"},
       {"x^2 - 3", "-1.73205080756888", "1/2", "-1/2*x"},
       {"x^6 - 7*x^4 + 14*x^2 - 7", "-1.94985582436365", "1/2*x^4 - 2*x^2 + 1",
        "-1/2*x^3 + 3/2*x"},
       {"x^2 - 2", "-1.41421356237310", "-1/2*x", "-1/2*x"},
       {"x^6 - 6*x^4 + 9*x^2 - 3", "-1.96961550602442", "1/2*x^4 - 2*x^2 + 1",
        "-1/2*x^5 + 5/2*x^3 - 5/2*x"},
       {"x^4 - 5*x^2 + 5", "-1.90211303259031", "1/2*x^2 - 1",
        "-1/2*x^3 + 3/2*x"},
       {"x^10 - 11*x^8 + 44*x^6 - 77*x^4 + 55*x^2 - 11", "-1.97964288376187",
        "1/2*x^4 - 2*x^2 + 1", "-1/2*x^7 + 7/2*x^5 - 7*x^3 + 7/2*x"},
       {"x^2 - 3", "-1.73205080756888", "-1/2*x", "1/2"},
       {"x^12 - 13*x^10 + 65*x^8 - 156*x^6 + 182*x^4 - 91*x^2 + 13",
        "-1.98541774819611", "1/2*x^4 - 2*x^2 + 1",
        "-1/2*x^9 + 9/2*x^7 - 27/2*x^5 + 15*x^3 - 9/2*x"},
       {"x^6 - 7*x^4 + 14*x^2 - 7", "-1.94985582436365", "1/2*x^2 - 1",
        "-1/2*x^5 + 5/2*x^3 - 5/2*x"},
       {"x^8 - 7*x^6 + 14*x^4 - 8*x^2 + 1", "-1.98904379073655",
        "1/2*x^4 - 2*x^2 + 1", "-x^7 + 13/2*x^5 - 11*x^3 + 7/2*x"},
       {"x^4 - 4*x^2 + 2", "-1.84775906502257", "-1/2*x", "-1/2*x^3 + 3/2*x"},
       {"x^16 - 17*x^14 + 119*x^12 - 442*x^10 + 935*x^8 - 1122*x^6 + 714*x^4 - "
        "204*x^2 + 17",
        "-1.99146835259007", "1/2*x^4 - 2*x^2 + 1",
        "-1/2*x^13 + 13/2*x^11 - 65/2*x^9 + 78*x^7 - 91*x^5 + 91/2*x^3 - "
        "13/2*x"},
       {"x^6 - 6*x^4 + 9*x^2 - 3", "-1.96961550602442", "1/2*x^2 - 1",
        "1/2*x^5 - 5/2*x^3 + 2*x"},
       {"x^18 - 19*x^16 + 152*x^14 - 665*x^12 + 1729*x^10 - 2717*x^8 + "
        "2508*x^6 - 1254*x^4 + 285*x^2 - 19",
        "-1.99316898601334", "1/2*x^4 - 2*x^2 + 1",
        "-1/2*x^15 + 15/2*x^13 - 45*x^11 + 275/2*x^9 - 225*x^7 + 189*x^5 - "
        "70*x^3 + 15/2*x"},
       {"x^4 - 5*x^2 + 5", "-1.90211303259031", "-1/2*x", "1/2*x^2 - 3/2"},
       {"x^12 - 11*x^10 + 44*x^8 - 78*x^6 + 60*x^4 - 16*x^2 + 1",
        "-1.99440759436236", "1/2*x^4 - 2*x^2 + 1",
        "1/2*x^11 - 11/2*x^9 + 22*x^7 - 77/2*x^5 + 27*x^3 - 4*x"},
       {"x^10 - 11*x^8 + 44*x^6 - 77*x^4 + 55*x^2 - 11", "-1.97964288376187",
        "1/2*x^2 - 1", "-1/2*x^9 + 9/2*x^7 - 27/2*x^5 + 15*x^3 - 9/2*x"},
       {"x^22 - 23*x^20 + 230*x^18 - 1311*x^16 + 4692*x^14 - 10948*x^12 + "
        "16744*x^10 - 16445*x^8 + 9867*x^6 - 3289*x^4 + 506*x^2 - 23",
        "-1.99533753838108", "1/2*x^4 - 2*x^2 + 1",
        "-1/2*x^19 + 19/2*x^17 - 76*x^15 + 665/2*x^13 - 1729/2*x^11 + "
        "2717/2*x^9 - 1254*x^7 + 627*x^5 - 285/2*x^3 + 19/2*x"},
       {"x^4 - 4*x^2 + 1", "-1.93185165257814", "-1/2*x", "1/2*x^3 - 2*x"},
       {"x^20 - 20*x^18 + 170*x^16 - 800*x^14 + 2275*x^12 - 4005*x^10 + "
        "4300*x^8 - 2675*x^6 + 875*x^4 - 125*x^2 + 5",
        "-1.99605345685654", "1/2*x^4 - 2*x^2 + 1",
        "1/2*x^19 - 19/2*x^17 + 76*x^15 - 665/2*x^13 + 864*x^11 - 2707/2*x^9 + "
        "2473/2*x^7 - 602*x^5 + 130*x^3 - 8*x"},
       {"x^12 - 13*x^10 + 65*x^8 - 156*x^6 + 182*x^4 - 91*x^2 + 13",
        "-1.98541774819611", "1/2*x^2 - 1",
        "-1/2*x^11 + 11/2*x^9 - 22*x^7 + 77/2*x^5 - 55/2*x^3 + 11/2*x"},
       {"x^18 - 18*x^16 + 135*x^14 - 546*x^12 + 1287*x^10 - 1782*x^8 + "
        "1386*x^6 - 540*x^4 + 81*x^2 - 3",
        "-1.99661631654254", "1/2*x^4 - 2*x^2 + 1",
        "1/2*x^13 - 13/2*x^11 + 65/2*x^9 - 78*x^7 + 181/2*x^5 - 43*x^3 + 4*x"},
       {"x^6 - 7*x^4 + 14*x^2 - 7", "-1.94985582436365", "-1/2*x",
        "1/2*x^4 - 5/2*x^2 + 5/2"},
       {"x^28 - 29*x^26 + 377*x^24 - 2900*x^22 + 14674*x^20 - 51359*x^18 + "
        "127281*x^16 - 224808*x^14 + 281010*x^12 - 243542*x^10 + 140998*x^8 - "
        "51272*x^6 + 10556*x^4 - 1015*x^2 + 29",
        "-1.99706682770225", "1/2*x^4 - 2*x^2 + 1",
        "-1/2*x^25 + 25/2*x^23 - 275/2*x^21 + 875*x^19 - 7125/2*x^17 + "
        "9690*x^15 - 17850*x^13 + 22100*x^11 - 17875*x^9 + 17875/2*x^7 - "
        "5005/2*x^5 + 325*x^3 - 25/2*x"},
       {"x^8 - 7*x^6 + 14*x^4 - 8*x^2 + 1", "-1.98904379073655", "1/2*x^2 - 1",
        "1/2*x^7 - 7/2*x^5 + 13/2*x^3 - 2*x"},
       {"x^30 - 31*x^28 + 434*x^26 - 3627*x^24 + 20150*x^22 - 78430*x^20 + "
        "219604*x^18 - 447051*x^16 + 660858*x^14 - 700910*x^12 + 520676*x^10 - "
        "260338*x^8 + 82212*x^6 - 14756*x^4 + 1240*x^2 - 31",
        "-1.99743301434211", "1/2*x^4 - 2*x^2 + 1",
        "-1/2*x^27 + 27/2*x^25 - 162*x^23 + 2277/2*x^21 - 10395/2*x^19 + "
        "32319/2*x^17 - 34884*x^15 + 52326*x^13 - 53703*x^11 + 36465*x^9 - "
        "15444*x^7 + 7371/2*x^5 - 819/2*x^3 + 27/2*x"}});

  if (n >= totally_real_data.size()) {
    throw new std::logic_error(
        "not implemented - totally real cyclotomic polynomials are hard coded "
        "and have not been generated for that degree yet");
  }

  // TODO: What is the correct precision here?
  auto totally_real_field = std::make_shared<eantic::renf_class>(
      totally_real_data[n][0], "x", totally_real_data[n][1] + " +/- 0.00001",
      flatsurf::VectorArb::prec);
  auto re =
      eantic::renf_elem_class(*totally_real_field, totally_real_data[n][2]);
  auto im =
      eantic::renf_elem_class(*totally_real_field, totally_real_data[n][3]);
  return std::make_tuple(std::move(totally_real_field), re, im);
}

template <typename T>
NumberField<T> *InitCyclotomic(int n) {
  // The cyclotomic field generated by the primitive n'th roots of unity
  NumberField<T> *F;

  complex<COORD> nth_rootofunity(
      cos(2 * boost::math::constants::pi<COORD>() / n),
      sin(2 * boost::math::constants::pi<COORD>() / n));
  poly<T> cyc = cyclotomic_poly<T>(n);
  vector<T> coeffs = cyc.coefficients;

  auto real_field = real_cyclotomic_field(numeric_cast<size_t>(n));
  F = new NumberField<T>(&coeffs[0], numeric_cast<size_t>(cyc.degree()),
                         nth_rootofunity, std::move(std::get<0>(real_field)),
                         std::complex<eantic::renf_elem_class>(
                             std::move(std::get<1>(real_field)),
                             std::move(std::get<2>(real_field))));
  F->store_conjugate(algebraic<T>(1, F).pow(n - 1));

  return (F);
}

template <typename T>
poly<T> &poly<T>::operator/=(T t) {
  for (auto &c : coefficients) {
    c /= t;
  }
  return *this;
}

template <typename T>
poly<T> poly<T>::operator()(const poly<T> &x) const {
  if (degree() <= 0) {
    return *this;
  }
  poly<T> ret;
  // This is very slow, but it should not matter as we hardly ever use it.
  for (int d = 0; d <= degree(); d++) {
    poly<T> c({coefficients[d]});
    for (int j = 0; j < d; j++) {
      c = c * x;
    }
    ret += c;
  }
  return ret;
}

template <typename T>
algebraic<T> alg_t<T>::get_coeff(size_t i) {
  return coeffs[i];
}

template <typename T>
void alg_t<T>::set_coeff(size_t i, const algebraic<T> &a) {
  coeffs[i] = a;
}

template <typename T>
alg_t<T>::alg_t() {
  coeffs.assign(Params::nbr_params() + 1, NumberField<T>::F_zero);
}

template <typename T>
void alg_t<T>::pad_coeffs(int n) {
  for (int i = 0; i < n; i++) {
    coeffs.push_back(algebraic<T>(NumberField<T>::F));  // set coefficients to 0
  }
}

template <typename T>
alg_t<T>::alg_t(vector<algebraic<T>> c) : coeffs(c) {
  if (c.size() != Params::nbr_params() + 1) {
    ERR_RET("alg_t: initialization with wrong length vector");
  }
}

namespace {
template <typename T, typename Ring>
std::vector<algebraic<T>> to_algebraic_coeffs(const VectorExactReal<Ring> &v) {
  exactreal::Element<Ring> x = v.x();
  exactreal::Element<Ring> y = v.y();

  x.promote(y.module());
  y.promote(x.module());
  assert(&*x.module() == &*y.module());

  return as_vector(irange(x.module()->rank()) | transformed([&](int i) {
                     return algebraic<T>(x[i], y[i]);
                   }));
}
}  // namespace

template <typename T>
template <typename Ring>
alg_t<T>::alg_t(const VectorExactReal<Ring> &v)
    : alg_t(to_algebraic_coeffs<T, Ring>(v)) {}

template <typename T>
alg_t<T> alg_t<T>::operator-() const {
  alg_t<T> r(*this);

  for (size_t i = 0; i <= Params::nbr_params(); i++) r.coeffs[i] = -(coeffs[i]);
  return r;
}

template <typename T>
alg_t<T> &alg_t<T>::operator+=(const alg_t<T> &p) {
  for (size_t i = 0; i <= Params::nbr_params(); i++) coeffs[i] += p.coeffs[i];
  return *this;
}

template <typename T>
alg_t<T> &alg_t<T>::operator-=(const alg_t<T> &p) {
  for (size_t i = 0; i <= Params::nbr_params(); i++) coeffs[i] -= p.coeffs[i];
  return *this;
}

template <typename T>
alg_t<T> operator+(alg_t<T> p, const alg_t<T> &q) {
  return p += q;
}

template <typename T>
alg_t<T> operator-(alg_t<T> p, const alg_t<T> &q) {
  return p -= q;
}

template <typename T>
alg_t<T> &alg_t<T>::operator*=(const algebraic<T> &r) {
  for (typename vector<algebraic<T>>::iterator i = coeffs.begin();
       i != coeffs.end(); ++i)
    *i *= r;
  return *this;
}

template <typename T>
alg_t<T> &alg_t<T>::operator*=(int r) {
  for (typename vector<algebraic<T>>::iterator i = coeffs.begin();
       i != coeffs.end(); ++i)
    *i *= r;
  return *this;
}

template <typename T>
alg_t<T> &alg_t<T>::operator/=(const algebraic<T> &r) {
  for (typename vector<algebraic<T>>::iterator i = coeffs.begin();
       i != coeffs.end(); ++i)
    *i /= r;
  return *this;
}

template <typename T>
alg_t<T> &alg_t<T>::operator/=(int r) {
  for (typename vector<algebraic<T>>::iterator i = coeffs.begin();
       i != coeffs.end(); ++i)
    *i /= r;
  return *this;
}

template <typename T>
alg_t<T> operator*(const algebraic<T> &p, alg_t<T> &q) {
  //    alg_t<T> r = q;
  return q *= p;
}

template <typename T>
alg_t<T> operator*(alg_t<T> q, const algebraic<T> &p) {
  //    alg_t<T> r = q;
  return q *= p;
}

template <typename T>
alg_t<T> operator*(int p, alg_t<T> &q) {
  //    alg_t<T> r = q;
  return q *= p;
}

template <typename T>
alg_t<T> operator*(alg_t<T> &q, int p) {
  //    alg_t<T> r = q;
  return q *= p;
}

template <typename T>
alg_t<T> operator/(alg_t<T> &q, const algebraic<T> &p) {
  //    alg_t<T> r = q;
  return q /= p;
}

template <typename T>
alg_t<T> operator/(alg_t<T> &q, int p) {
  //    alg_t<T> r = q;
  return q /= p;
}

template <typename T>
bool operator==(const alg_t<T> &p, const alg_t<T> &q) {
  if (p.coeffs == q.coeffs)
    return true;
  else
    return false;
}

template <typename T>
bool operator!=(const alg_t<T> &p, const alg_t<T> &q) {
  return (!(p == q));
}

template <typename T>
Point alg_t<T>::tocomplex() const {
  Point b = coeffs[0].tocomplex();
  for (size_t i = 1; i <= Params::nbr_params(); i++)
    b += coeffs[i].tocomplex() * Params::params[i - 1];
  return b;
}

template <typename T>
bool alg_t<T>::get_direction(algebraic<T> &direction) {
  algebraic<T> q;
  algebraic<T> zero_F = algebraic<T>(coeffs[0].field());
  for (size_t i = 0; i <= Params::nbr_params(); i++) {
    for (size_t j = i + 1; j <= Params::nbr_params(); j++) {
      q = coeffs[i] * coeffs[j].conjugate() - coeffs[j] * coeffs[i].conjugate();
      if (q != zero_F) {  // q != 0
        return (false);
      }
    }
  }
  for (size_t i = 0; i <= Params::nbr_params(); i++) {
    q = coeffs[i];
    if (q != zero_F) {
      break;
    }
  }
  if (q == zero_F) {
    ERR_RET("alg_t: get_direction called on zero vector");
  }
  direction = q;
  return (true);
}

template <typename T>
bool colinear(const alg_t<T> &p1, const alg_t<T> &p2) {
  algebraic<T> q;  // initialized to 0

  for (size_t i = 0; i <= Params::nbr_params(); i++) {
    for (size_t j = i; j <= Params::nbr_params(); j++) {
      q = cross_product(p1.coeffs[i], p2.coeffs[j]);

      q += cross_product(p1.coeffs[j], p2.coeffs[i]);

      if (q != NumberField<T>::F_zero) {  // q != 0
        return (false);
      }
    }
  }
  return (true);
}

template <typename T>
Element<NumberFieldTraits> alg_t<T>::real() const {
  return Element<NumberFieldTraits>(
      Params::module(), as_vector(coeffs | transformed([](const auto &coeff) {
                                    return coeff.real();
                                  })));
}

template <typename T>
Element<NumberFieldTraits> alg_t<T>::imag() const {
  return Element<NumberFieldTraits>(
      Params::module(), as_vector(coeffs | transformed([](const auto &coeff) {
                                    return coeff.imag();
                                  })));
}

template <typename T>
ostream &operator<<(ostream &outputStream, const alg_t<T> &u) {
  outputStream << u.coeffs[0];
  for (size_t i = 1; i <= Params::nbr_params(); i++) {
    outputStream << "+" << u.coeffs[i] << "t" << i;
  }
  return outputStream;
}

// bigpoint stuff

template <typename T>
BigPoint<T>::BigPoint() {
  cx = 0;
}

template <typename T>
BigPoint<T>::BigPoint(int a, int b) {
  if (a != 0 || b != 0) {
    ERR_RET("BigPoint: non-zero initialization not implemented");
  }

  cx = 0;
  algt = alg_t<T>();
}

template <typename T>
BigPoint<T>::BigPoint(alg_t<T> u) : algt(u) {
  cx = u.tocomplex();
}

template <typename T>
void BigPoint<T>::SetCxFromAlgebraic() {
  cx = algt.tocomplex();
}

template <typename T>
BigPoint<T> BigPoint<T>::operator-() {
  BigPoint<T> r = *this;
  r.cx = -r.cx;
  r.algt = -r.algt;

  return r;
}

template <typename T>
BigPoint<T> &BigPoint<T>::operator+=(const BigPoint<T> &p) {
  cx += p.cx;
  algt += p.algt;

  return *this;
}

template <typename T>
BigPoint<T> &BigPoint<T>::operator-=(const BigPoint<T> &p) {
  cx -= p.cx;
  algt -= p.algt;

  return *this;
}

template <typename T>
BigPoint<T> operator+(BigPoint<T> p, const BigPoint<T> &q) {
  return p += q;
}

template <typename T>
BigPoint<T> operator-(BigPoint<T> p, const BigPoint<T> &q) {
  return p -= q;
}

template <typename T>
BigPoint<T> &BigPoint<T>::operator*=(const algebraic<T> &r) {
  cx = r.tocomplex() * cx;
  algt *= r;
  return *this;
}

template <typename T>
BigPoint<T> &BigPoint<T>::operator*=(int r) {
  cx = static_cast<COORD>(r) * cx;
  algt *= r;
  return *this;
}

template <typename T>
BigPoint<T> &BigPoint<T>::operator/=(const algebraic<T> &r) {
  cx /= r.tocomplex();
  algt /= r;
  return *this;
}

template <typename T>
BigPoint<T> &BigPoint<T>::operator/=(int r) {
  cx /= (1.0 * r);
  algt /= r;
  return *this;
}

template <typename T>
BigPoint<T> operator*(const algebraic<T> &p, BigPoint<T> q) {
  BigPoint<T> r = q;
  return r *= p;
}

template <typename T>
BigPoint<T> operator*(BigPoint<T> q, const algebraic<T> &p) {
  return q *= p;
}

template <typename T>
BigPoint<T> operator*(int p, BigPoint<T> q) {
  return q *= p;
}

template <typename T>
BigPoint<T> operator*(BigPoint<T> q, int p) {
  return q *= p;
}

template <typename T>
BigPoint<T> operator/(BigPoint<T> q, const algebraic<T> &p) {
  return q /= p;
}

template <typename T>
BigPoint<T> operator/(BigPoint<T> q, int p) {
  return q /= p;
}

template <typename T>
void BigPoint<T>::Check() {
  if (field_arithmetic && abs(cx - algt.tocomplex()) > EPSILON) {
    ERR_RET("BigPoint<T> Check Failed");
  }
}

template <typename T>
bool operator==(const BigPoint<T> &p, const BigPoint<T> &q) {
  if (field_arithmetic) {
    if (p.algt == q.algt) {
      return true;
    } else {
      return false;
    }
  } else {
    if (norm(p.cx - q.cx) < EPSILON * EPSILON) {
      return true;
    } else {
      return false;
    }
  }
}

template <typename T>
alg_t<T> alg_t<T>::conjugate() const {
  alg_t<T> conj;
  for (size_t i = 0; i <= Params::nbr_params(); i++) {
    conj.coeffs[i] = coeffs[i].conjugate();
  }
  return conj;
}

template <typename T>
ostream &operator<<(ostream &outputStream, const BigPoint<T> &p) {
  outputStream << p.cx << "=" << p.algt;
  return outputStream;
}
}  // namespace polygon

// Explicit template instantiations for mpq
namespace polygon {
template class BigPoint<bigrat>;
template class alg_t<bigrat>;

template poly<bigrat> cyclotomic_poly(int n);
template NumberField<bigrat> *InitCyclotomic(int n);
template alg_t<bigrat> operator+(alg_t<bigrat> p, const alg_t<bigrat> &q);
template alg_t<bigrat> operator-(alg_t<bigrat> p, const alg_t<bigrat> &q);
template alg_t<bigrat> operator*(const algebraic<bigrat> &p, alg_t<bigrat> &q);
template alg_t<bigrat> operator*(alg_t<bigrat> q, const algebraic<bigrat> &p);
template alg_t<bigrat> operator*(int p, alg_t<bigrat> &q);
template alg_t<bigrat> operator*(alg_t<bigrat> &q, int p);
template alg_t<bigrat> operator/(alg_t<bigrat> &q, const algebraic<bigrat> &p);
template alg_t<bigrat> operator/(alg_t<bigrat> &q, int p);
template bool operator==(const alg_t<bigrat> &p, const alg_t<bigrat> &q);
template bool operator!=(const alg_t<bigrat> &p, const alg_t<bigrat> &q);
template bool colinear(const alg_t<bigrat> &p1, const alg_t<bigrat> &p2);
template ostream &operator<<(ostream &outputStream, const alg_t<bigrat> &u);

template BigPoint<bigrat> operator+(BigPoint<bigrat> p,
                                    const BigPoint<bigrat> &q);
template BigPoint<bigrat> operator-(BigPoint<bigrat> p,
                                    const BigPoint<bigrat> &q);
template BigPoint<bigrat> operator*(const algebraic<bigrat> &p,
                                    BigPoint<bigrat> q);
template BigPoint<bigrat> operator*(BigPoint<bigrat> q,
                                    const algebraic<bigrat> &p);
template BigPoint<bigrat> operator*(int p, BigPoint<bigrat> q);
template BigPoint<bigrat> operator*(BigPoint<bigrat> q, int p);
template BigPoint<bigrat> operator/(BigPoint<bigrat> q,
                                    const algebraic<bigrat> &p);
template BigPoint<bigrat> operator/(BigPoint<bigrat> q, int p);
template bool operator==(const BigPoint<bigrat> &p, const BigPoint<bigrat> &q);
template ostream &operator<<(ostream &outputStream, const BigPoint<bigrat> &p);
}  // namespace polygon

// Explicit template instantiations for int64
namespace polygon {
template class BigPoint<int64_t>;
template class alg_t<int64_t>;

template poly<int64_t> cyclotomic_poly(int n);
template NumberField<int64_t> *InitCyclotomic(int n);
template alg_t<int64_t>::alg_t(const VectorExactReal<NumberFieldTraits> &);
template alg_t<int64_t> operator+(alg_t<int64_t> p, const alg_t<int64_t> &q);
template alg_t<int64_t> operator-(alg_t<int64_t> p, const alg_t<int64_t> &q);
template alg_t<int64_t> operator*(const algebraic<int64_t> &p,
                                  alg_t<int64_t> &q);
template alg_t<int64_t> operator*(alg_t<int64_t> q,
                                  const algebraic<int64_t> &p);
template alg_t<int64_t> operator*(int p, alg_t<int64_t> &q);
template alg_t<int64_t> operator*(alg_t<int64_t> &q, int p);
template alg_t<int64_t> operator/(alg_t<int64_t> &q,
                                  const algebraic<int64_t> &p);
template alg_t<int64_t> operator/(alg_t<int64_t> &q, int p);
template bool operator==(const alg_t<int64_t> &p, const alg_t<int64_t> &q);
template bool operator!=(const alg_t<int64_t> &p, const alg_t<int64_t> &q);
template bool colinear(const alg_t<int64_t> &p1, const alg_t<int64_t> &p2);
template ostream &operator<<(ostream &outputStream, const alg_t<int64_t> &u);

template BigPoint<int64_t> operator+(BigPoint<int64_t> p,
                                     const BigPoint<int64_t> &q);
template BigPoint<int64_t> operator-(BigPoint<int64_t> p,
                                     const BigPoint<int64_t> &q);
template BigPoint<int64_t> operator*(const algebraic<int64_t> &p,
                                     BigPoint<int64_t> q);
template BigPoint<int64_t> operator*(BigPoint<int64_t> q,
                                     const algebraic<int64_t> &p);
template BigPoint<int64_t> operator*(int p, BigPoint<int64_t> q);
template BigPoint<int64_t> operator*(BigPoint<int64_t> q, int p);
template BigPoint<int64_t> operator/(BigPoint<int64_t> q,
                                     const algebraic<int64_t> &p);
template BigPoint<int64_t> operator/(BigPoint<int64_t> q, int p);
template bool operator==(const BigPoint<int64_t> &p,
                         const BigPoint<int64_t> &q);
template ostream &operator<<(ostream &outputStream, const BigPoint<int64_t> &p);
}  // namespace polygon
