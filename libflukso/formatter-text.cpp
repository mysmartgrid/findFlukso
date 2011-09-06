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

#include "formatter-text.hpp"

using namespace Flukso;

void TextFormatter::exportTimeseries(std::ostream& os) const {
  Flukso::Timeseries::const_iterator ts_it;
  for( ts_it = _values->begin(); ts_it != _values->end(); ts_it++) {
	os << (*ts_it).first << "\t" << (*ts_it).second << std::endl;
  }
}
