/**
 * This file is part of libflukso.
 *
 * (c) Mathias Dalheimer <md@gonium.net>, 2010
 *
 * libflukso is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * libflukso is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libflukso. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "formatter-chumby-current.hpp"
#include <algorithm>


using namespace Flukso;

void ChumbyCurrentFormatter::exportTimeseries(std::ostream& os) const {
  Flukso::Timeseries::const_iterator ts_it;

  os << "<?xml version=\"1.0\"?>" << std::endl;
  os << "<current_reading><reading>" << std::endl;
  // look for latest timestamp in the _values map
  ts_it = std::max_element(_values->begin(), _values->end());
  os << "  <time>"<<convertTimestamp((*ts_it).first)<<"</time><value>"
    <<(*ts_it).second<<"</value>" << std::endl;
  os << "</reading></current_reading>" << std::endl;
}
