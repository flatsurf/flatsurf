/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2018 Alex Eskin
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

#include <boost/lexical_cast.hpp>
#include <exact-real/real_number.hpp>
#include <ostream>
#include <vector>

#include "./number_field.h"
#include "./params.h"
#include "flatsurf/vector_arb.hpp"

using exactreal::RealNumber;
using flatsurf::VectorArb;
using std::endl;
using std::ostream;
using std::shared_ptr;
using std::vector;

namespace {
vector<shared_ptr<RealNumber>> reals;
// TODO: Should I fix a prec here?
auto mod = std::make_shared<const Module<NumberFieldTraits>>(
    std::vector<std::shared_ptr<RealNumber>>{RealNumber::rational(1)},
    eantic::renf_class());
}  // namespace

namespace polygon {
size_t Params::n_params = 0;
vector<COORD> Params::params;

size_t Params::nbr_params() { return n_params; }

void Params::AddParams(size_t n, COORD prms[]) {
  for (size_t i = 0; i < n; i++) {
    params.push_back(prms[i]);
    reals.push_back(shared_ptr<RealNumber>(RealNumber::random(prms[i])));
  }
  n_params += n;
  vector<shared_ptr<RealNumber>> basis;
  basis.push_back(RealNumber::rational(1));
  for (const auto &b : reals) {
    basis.push_back(b);
  }
  // TODO: Should I fix a prec here?
  mod = std::make_shared<const Module<NumberFieldTraits>>(
      basis, static_cast<const eantic::renf_class &>(*NumberField<bigrat>::F));
}

void Params::print(ostream &out) {
  out << "Transcendental Parameters:";
  for (size_t i = 0; i < nbr_params(); i++) {
    out << " " << *reals[i];
  }
  out << endl;
}

std::shared_ptr<const Module<NumberFieldTraits>> Params::module() {
  return mod;
}

shared_ptr<RealNumber> Params::exact(size_t i) { return reals.at(i); }
}  // namespace polygon
