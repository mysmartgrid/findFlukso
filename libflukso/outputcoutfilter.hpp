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

#ifndef LIBFLUKSO_OUTPUTCOUTFILTER_HPP
#define LIBFLUKSO_OUTPUTCOUTFILTER_HPP 1

#include <outputfilter.hpp>
#include <iostream>

namespace Flukso {
  class OutputCoutFilter : public OutputFilter{
	public:
	  typedef std::tr1::shared_ptr<OutputCoutFilter> Ptr;
	  explicit OutputCoutFilter () : Flukso::OutputFilter() {};
	  virtual ~OutputCoutFilter() {};
	  virtual void render(const Flukso::Formatter::Ptr formatter) {
		std::cout << *formatter;
	  };

	private:
	  OutputCoutFilter (const OutputCoutFilter& original);
	  OutputCoutFilter& operator= (const OutputCoutFilter& rhs);
  };
}

#endif //LIBFLUKSO_OUTPUTCOUTFILTER_HPP
