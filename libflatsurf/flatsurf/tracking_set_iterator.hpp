/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020 Julian Rüth
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

#ifndef LIBFLATSURF_TRACKING_SET_ITERATOR_HPP
#define LIBFLATSURF_TRACKING_SET_ITERATOR_HPP

#include <boost/iterator/iterator_facade.hpp>

#include "external/spimpl/spimpl.h"

#include "forward.hpp"

namespace flatsurf {

template <typename T>
class TrackingSetIterator : public boost::iterator_facade<TrackingSetIterator<T>, const T&, boost::forward_traversal_tag> {
    // Cannot be created directly. Use begin() and end() on a TrackingSet to create instances of a TrackingSetIterator.
    TrackingSetIterator();

  public:
    using value_type = T;

    void increment();
    const T &dereference() const;
    bool equal(const TrackingSetIterator &other) const;

    template <typename S>
    friend std::ostream& operator<<(std::ostream&, const TrackingSetIterator<S>&);

  private:
    using Implementation = ::flatsurf::Implementation<TrackingSetIterator<T>>;
    spimpl::impl_ptr<Implementation> impl;
    friend Implementation;
};

}

#endif
