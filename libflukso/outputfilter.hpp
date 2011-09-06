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

#ifndef LIBFLUKSO_OUTPUTFILTER_HPP
#define LIBFLUKSO_OUTPUTFILTER_HPP 1

#include <formatter.hpp>
#include <error.hpp>

namespace Flukso {
  class OutputFilter {
	public:
	  typedef std::tr1::shared_ptr<OutputFilter> Ptr;
	  explicit OutputFilter () {};
	  virtual ~OutputFilter() {};
	  virtual void render(const Flukso::Formatter::Ptr formatter) = 0;

	  static OutputFilter::Ptr buildFilter(
		  const Config::Ptr& config) throw(ConfigurationException);

	private:
	  OutputFilter (const OutputFilter& original);
	  OutputFilter& operator= (const OutputFilter& rhs);
	  
  };
  
};


#endif /* LIBFLUKSO_OUTPUTFILTER_HPP */

