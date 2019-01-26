/**********************************************************************
 *  This file is part of Polygon.
 *
 *        Copyright (C) 2018 Alex Eskin
 *        Copyright (C) 2019 Vincent Delecroix
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

#include <NTL/LLL.h>
#include <NTL/mat_ZZ.h>
#include <iostream>
#include <vector>
#include <boost/math/constants/constants.hpp>

#include "libpolygon/globals.h"
#include "libpolygon/shared.h"
#include "libpolygon/number_field.h"
#include "libpolygon/oedge.h"
#include "libpolygon/params.h"
#include "libpolygon/two_complex.h"
#include "libpolygon/uedge.h"

using namespace NTL;
using std::abs;
using std::cout;
using std::endl;
using std::vector;
using boost::math::constants::pi;
using boost::numeric_cast;
using std::uniform_real_distribution;

namespace polygon {
void TwoComplex::PerturbConjugates(COORD max_perturb) {
  size_t n = Params::nbr_params();

  std::cout << "max_perturb = " << max_perturb << endl;

  vector<COORD> perturb;

  COORD base = 3.1 / pi<COORD>();

  COORD current = base;
  for (size_t i = 0; i < 2 * n; i++) {
    perturb.push_back(current * max_perturb);  // hack
    current *= base;
  }
  size_t count = 0;
  size_t n_orig = n;

  for (size_t i = 1; i <= n_orig; i++) {
    // want to add conjugate of ith deformation
    bool already_have_conjugate = false;
    for (size_t j = i + 1; j <= n_orig; j++) {
      if (is_conjugate_deformation(i, j)) {
        already_have_conjugate = true;
        break;
      }
    }
    if (already_have_conjugate) {
      continue;
    }

    std::cout << perturb[0];

    Params::AddParams(1u, &(perturb[count]));
    n++;
    count++;
    for (auto j = S->uedges.begin(); j != S->uedges.end(); ++j) {
      alg_tQ tmp = (*j)->ue_vecQ.algt;
      tmp.pad_coeffs(1);  // probably not needed
      algebraicQ tmp2 = tmp.get_coeff(i);
      tmp.set_coeff(n, tmp2.conjugate());
      (*j)->ue_vecQ.algt = tmp;
      (*j)->ue_vecQ.cx = tmp.tocomplex();
    }
  }
}

bool TwoComplex::is_conjugate_deformation(size_t j, size_t k) {
  for (auto i = S->uedges.begin(); i != S->uedges.end(); ++i) {
    if ((*i)->ue_vecQ.algt.get_coeff(j) !=
        (*i)->ue_vecQ.algt.get_coeff(k).conjugate()) {
      return false;
    }
  }
  return (true);
}

void Vertex::MoveVertex(Point p) {
  size_t n = Params::nbr_params();

  if (field_arithmetic) {
    COORD tmp[2];

    algebraicQ xi =
        algebraicQ(1, NumberField<bigrat>::F);  // the generator of F
    COORD xi_real = xi.tocomplex().real();
    COORD xi_imag = xi.tocomplex().imag();

    // perturbation is tmp[0] xi + tmp[1] \bar{\xi}
    // want this equal to p, while tmp[0] and tmp[1] are real

    // tmp[0]*xi + tmp[1]*\bar{\xi} = p
    // tmp[0]*\bar{xi} + tmp[1]*\xi = \bar{p}

    //(bar{\xi} - \xi)/(2i) = -xi_imag

    // tmp[0]*xi_real + tmp[1]*xi_real = p.real();
    // tmp[0]*xi_imag - tmp[1]*xi_imag = p.imag();

    tmp[0] = 0.5 * p.real() / xi_real + 0.5 * p.imag() / xi_imag;
    tmp[1] = 0.5 * p.real() / xi_real - 0.5 * p.imag() / xi_imag;

    Params::AddParams(2, tmp);
    for (auto i = S->uedges.begin(); i != S->uedges.end(); ++i) {
      (*i)->ue_vecQ.algt.pad_coeffs(2);
    }
  }

  for (auto i = out_edges.begin(); i != out_edges.end(); ++i) {
    Point q;
    q = (*i)->vec_cx();
    q = q + p;
    (*i)->set_vec_cx(q);

    if (field_arithmetic) {
      algebraicQ xi = algebraicQ(1, NumberField<bigrat>::F);
      alg_tQ tmp = (*i)->vec_algt();
      tmp.set_coeff(n + 1, xi);
      tmp.set_coeff(n + 2, xi.conjugate());
      (*i)->set_vec_algt(tmp);

      (*i)->vecQ().Check();
    }
  }
}

void TwoComplex::PerturbAll(COORD max_perturb) {
  UEdge **uedge_array = new UEdge *[uedges.size() + 1];

	{
		size_t i = 1;
		for (auto j = uedges.begin(); j != uedges.end(); ++j) {
			uedge_array[i] = (*j);
			(*j)->index = numeric_cast<int>(i);

			i++;
		}
	}

  size_t M, N;

  M = faces.size() + vertices.size();
  N = uedges.size();

  mat_ZZ X;
  X.SetDims(static_cast<long>(M), static_cast<long>(N));
  mat_ZZ Y;
  Y.SetDims(static_cast<long>(N), static_cast<long>(M));

  //,u,v,w;

  for (size_t i = 1; i <= M; i++) {
    for (size_t k = 1; k <= N; k++) {
      X(static_cast<long>(i), static_cast<long>(k)) = 0;
    }
  }

	{
		size_t i = 1;
		for (auto l = faces.begin(); l != faces.end(); ++l) {
			for (auto j = (*l)->oedges.begin(); j != (*l)->oedges.end(); ++j) {
				X(static_cast<long>(i), (*j).ue->index) += (*j).direction;
			}
			i++;
		}
		/* triangles assumed here */
		for (auto l = vertices.begin(); l != vertices.end(); ++l) {
			for (auto j = (*l)->out_edges.begin(); j != (*l)->out_edges.end(); ++j) {
				X(static_cast<long>(i), (*j)->next_edge()->ue->index) += (*j)->next_edge()->direction;
			}
			i++;
		}
	}

	{
		for (size_t i = 1; i <= M; i++) {
			for (size_t k = 1; k <= N; k++) {
				Y(static_cast<long>(k), static_cast<long>(i)) = X(static_cast<long>(i), static_cast<long>(k));
			}
		}
	}

  ZZ det2;
  mat_ZZ U;
  U.SetDims(static_cast<long>(N), static_cast<long>(N));
  long r = LLL(det2, Y, U);

	{
		long i = static_cast<long>(vertices.size());
		for (auto j = vertices.begin(); j != vertices.end(); ++j) {
			if (abs((*j)->total_angle() - 2 * pi<COORD>()) > EPSILON) --i;
		}
		fprintf(out_f, "rank = %lu #of parameters %ld\n", r, numeric_cast<long>(N) - r - i);
	}

  Point *perturb_array = new Point[N + 1];
  Point *delta_array = new Point[N + 1];

  COORD *norms = new COORD[N + 1];

  for (size_t i = 1; i <= N; i++) {
    norms[i] = 0.0;
  }

  for (size_t i = 1; i <= N; i++) {
    delta_array[i] = 0.0;
  }

  for (size_t i = 1; i <= N; i++) {
    perturb_array[i] = Point(max_perturb * std::uniform_real_distribution<double>(0, 1)(random_engine),
                             max_perturb * std::uniform_real_distribution<double>(0, 1)(random_engine));
    ;
  }

  for (long k = 1; k <= static_cast<long>(N) - r; k++) {
    for (size_t i = 1; i <= N; i++) {
      norms[k] += static_cast<COORD>((1.0L) * to_long(U(k, static_cast<long>(i))) * to_long(U(k, static_cast<long>(i))));
    }
  }

  for (long k = 1; k <= static_cast<long>(N) - r; k++) {
    for (size_t i = 1; i <= N; i++) {
      delta_array[i] +=
          perturb_array[k] * Point(numeric_cast<double>(to_long(U(k, static_cast<long>(i))))) / sqrt(norms[k]);
    }
  }

  for (size_t i = 1; i <= N; i++) {
    std::cout << "Edge " << i << " moving by " << delta_array[i] << "\n";
    uedge_array[i]->ue_vecQ.cx += delta_array[i];
  }
}  // main
}  // namespace polygon
