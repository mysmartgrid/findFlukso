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


#include <common.hpp>
#include "formatter.hpp"
#include <formatter-text.hpp>
#include <formatter-xml.hpp>
#include <formatter-chumby-current.hpp>
#include <formatter-chumby-lastminute.hpp>
#include <formatter-chumby-lasthour.hpp>
#include <formatter-chumby-lastday.hpp>
#include <sstream>
#include <ctime>

namespace Flukso {

  Formatter::Ptr Formatter::buildFormatter(
	  const Config::Ptr& config,
	  const TimeseriesPtr& values) throw(ConfigurationException){
	std::string config_option = config->getFormatterType();
	if (config_option == std::string("text")) {
	  return Flukso::Formatter::Ptr (new TextFormatter(values));
	} else if (config_option == std::string("xml")) {
	  return Flukso::Formatter::Ptr (new XMLFormatter(values));
	} else if (config_option == std::string("chumby-current")) {
	  return Flukso::Formatter::Ptr (new ChumbyCurrentFormatter(values));
  } else if (config_option == std::string("chumby-lastminute")) {
	  return Flukso::Formatter::Ptr (new ChumbyLastMinuteFormatter(values));
  } else if (config_option == std::string("chumby-lasthour")) {
	  return Flukso::Formatter::Ptr (new ChumbyLastHourFormatter(values));
  } else if (config_option == std::string("chumby-lastday")) {
	  return Flukso::Formatter::Ptr (new ChumbyLastDayFormatter(values));
	} else {
	  std::ostringstream oss;
	  oss << "Invalid formatter type \""<< config_option << "\"";
	  throw ConfigurationException(oss.str());
	}
  }


  std::string Formatter::convertTimestamp(const long& timestamp) const{
    time_t ts=timestamp;
    struct tm* timeinfo;
    char buffer[80];

    timeinfo = localtime ( &ts);
    strftime (buffer,80,"%H:%M:%S",timeinfo);
    
    return std::string(buffer);
  }
}
