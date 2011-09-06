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

#include "outputfilter.hpp"
#include <outputcoutfilter.hpp>
#include <outputfilefilter.hpp>
#include <sstream>



namespace Flukso {
  OutputFilter::Ptr OutputFilter::buildFilter(
	  const Config::Ptr& config) throw(ConfigurationException) 
  {
    std::string filter_option = config->getFilterType();
    if (filter_option == std::string("cout")) {
      return Flukso::OutputFilter::Ptr (new Flukso::OutputCoutFilter());
    } else if (filter_option == std::string("file")) {
      std::string file_option = config->getOutputFilename();
      return Flukso::OutputFilter::Ptr (new Flukso::OutputFileFilter(file_option));
    } else {
      std::ostringstream oss;
      oss << "Invalid output filter type \""<< filter_option << "\"";
      throw ConfigurationException(oss.str());
    }
  }
}
