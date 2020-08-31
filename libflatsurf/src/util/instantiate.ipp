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

#ifndef LIBFLATSURF_UTIL_INSTANTIATE_IPP
#define LIBFLATSURF_UTIL_INSTANTIATE_IPP

#include <e-antic/renfxx.h>
#include <gmpxx.h>

#include <boost/preprocessor/seq/fold_left.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_product.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/transform.hpp>
#include <exact-real/element.hpp>
#include <exact-real/integer_ring.hpp>
#include <exact-real/number_field.hpp>
#include <exact-real/rational_field.hpp>

#include "../../flatsurf/flat_triangulation.hpp"
#include "../../flatsurf/flat_triangulation_collapsed.hpp"

#define LIBFLATSURF_REM_(...) __VA_ARGS__
#define LIBFLATSURF_REM(ARGS) LIBFLATSURF_REM_ ARGS

#define LIBFLATSURF_REAL_TYPES (long long)(mpz_class)(mpq_class)(eantic::renf_elem_class)(exactreal::Element<exactreal::IntegerRing>)(exactreal::Element<exactreal::RationalField>)(exactreal::Element<exactreal::NumberField>)

#define LIBFLATSURF_SURFACE_TYPE_TEMPLATES (FlatTriangulation)(FlatTriangulationCollapsed)

#define LIBFLATSURF_WRAP(R, TYPE, T) (TYPE<T>)

#define LIBFLATSURF_SURFACE_TYPE_TEMPLATES_WRAP(R, PRODUCT) (BOOST_PP_SEQ_ELEM(0, PRODUCT) < BOOST_PP_SEQ_ELEM(1, PRODUCT) >)

#define LIBFLATSURF_SURFACE_TYPES BOOST_PP_SEQ_FOR_EACH_PRODUCT(LIBFLATSURF_SURFACE_TYPE_TEMPLATES_WRAP, (LIBFLATSURF_SURFACE_TYPE_TEMPLATES)(LIBFLATSURF_REAL_TYPES))

#define LIBFLATSURF_FLAT_TRIANGULATION_TYPES BOOST_PP_SEQ_FOR_EACH_PRODUCT(LIBFLATSURF_SURFACE_TYPE_TEMPLATES_WRAP, ((FlatTriangulation))(LIBFLATSURF_REAL_TYPES))

#define LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION(T) \
  namespace flatsurf {                                 \
  template class ImplementationOf<LIBFLATSURF_REM(T)>; \
  }                                                    \
  LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION(T)

#define LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION(T)                       \
  namespace flatsurf {                                                          \
  template class LIBFLATSURF_REM(T);                                            \
  template std::ostream& operator<<(std::ostream&, const LIBFLATSURF_REM(T) &); \
  }

#define LIBFLATSURF_INSTANTIATE_STATIC(T) \
  namespace flatsurf {                    \
  template class LIBFLATSURF_REM(T);      \
  }

#define LIBFLATSURF_INSTANTIATE_HASH(T)     \
  namespace std {                           \
  using namespace flatsurf;                 \
  template struct hash<LIBFLATSURF_REM(T)>; \
  }

// LIBFLATSURF_INSTANTIATE_TEMPLATE(TEMP, (T))
#define LIBFLATSURF_INSTANTIATE_TEMPLATE(TEMPLATE, TYPE) \
  TEMPLATE(TYPE)

#define LIBFLATSURF_INSTANTIATE_TEMPLATE_R_I(R, TYPE, I, TEMPLATE) \
  LIBFLATSURF_INSTANTIATE_TEMPLATE(TEMPLATE, TYPE)

// LIBFLATSURF_INSTANTIATE((TEMP1)(TEMP2), (T))
#define LIBFLATSURF_INSTANTIATE(TEMPLATES, TYPE) \
  BOOST_PP_SEQ_FOR_EACH_I(LIBFLATSURF_INSTANTIATE_TEMPLATE_R_I, TYPE, TEMPLATES)

#define LIBFLATSURF_INSTANTIATE_R(R, TEMPLATES, TYPE) \
  LIBFLATSURF_INSTANTIATE(TEMPLATES, TYPE)

// LIBFLATSURF_INSTANTIATE_MANY((TEMP1)(TEMP2), ((T1))((T2)))
#define LIBFLATSURF_INSTANTIATE_MANY(TEMPLATES, TYPES) \
  BOOST_PP_SEQ_FOR_EACH(LIBFLATSURF_INSTANTIATE_R, TEMPLATES, TYPES)

// LIBFLATSURF_INSTANTIATE_MANY_FROM_TRANSFORMATION((TEMP1)(TEMP2), T, (T1)(T2), TRANSFORMATION) -> TEMP1<TRANSFORMATION(T, T1)>, …, TEMP2<TRANSFORMATION(T, T2)>
#define LIBFLATSURF_INSTANTIATE_MANY_FROM_TRANSFORMATION(TEMPLATES, T, TYPES, TRANSFORMATION) \
  LIBFLATSURF_INSTANTIATE_MANY(TEMPLATES, BOOST_PP_SEQ_TRANSFORM(TRANSFORMATION, T, TYPES))

// LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((TEMP1)(TEMP2), T, (T1)(T2)) -> TEMP1(T<T1>), …, TEMP2(T<T2>)
#define LIBFLATSURF_INSTANTIATE_MANY_WRAPPED(TEMPLATES, T, TYPES) \
  LIBFLATSURF_INSTANTIATE_MANY_FROM_TRANSFORMATION(TEMPLATES, T, TYPES, LIBFLATSURF_WRAP)

#endif
