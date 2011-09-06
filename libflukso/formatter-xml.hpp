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


#ifndef LIBFLUKSO_EXPORTER_XML_HPP
#define LIBFLUKSO_EXPORTER_XML_HPP 1

#include <iostream>
#include <flukso.hpp>
#include <formatter.hpp>

namespace Flukso {
  class XMLFormatter : public Flukso::Formatter {
	public:
	  typedef std::tr1::shared_ptr<XMLFormatter> Ptr;
	  XMLFormatter (Flukso::TimeseriesPtr values) 
		: Flukso::Formatter(values) {};
	  virtual ~XMLFormatter() {};
	  void exportTimeseries(std::ostream& os) const;
	  virtual const std::string getType() const {return std::string("xml"); };

	private:
	  XMLFormatter (const XMLFormatter& original);
	  XMLFormatter& operator= (const XMLFormatter& rhs);
	  
  };
  
}


#endif /* LIBFLUKSO_EXPORTER-XML_HPP */

