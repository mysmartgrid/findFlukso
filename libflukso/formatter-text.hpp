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


#ifndef LIBFLUKSO_EXPORTER_TEXT_HPP
#define LIBFLUKSO_EXPORTER_TEXT_HPP 1

#include <iostream>
#include <flukso.hpp>
#include <formatter.hpp>

namespace Flukso {
  class TextFormatter : public Flukso::Formatter {
	public:
	  typedef std::tr1::shared_ptr<TextFormatter> Ptr;
	  explicit TextFormatter (Flukso::TimeseriesPtr values) 
		: Flukso::Formatter(values) { };
	  virtual ~TextFormatter() {};
	  void exportTimeseries(std::ostream& os) const;
	  virtual const std::string getType() const {return std::string("text"); };

	private:
	  TextFormatter (const TextFormatter& original);
	  TextFormatter& operator= (const TextFormatter& rhs);
  };

}

#endif /* LIBFLUKSO_EXPORTER_TEXT_HPP */
