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

#ifndef LIBFLUKSO_FLUKSO_HPP
#define LIBFLUKSO_FLUKSO_HPP 1

#include <common.hpp>
#include <config.hpp>
#include <curl/curl.h>
#include <error.hpp>
#include <map>

namespace Flukso {
  typedef std::map<long,long> Timeseries;
  typedef std::tr1::shared_ptr<Timeseries> TimeseriesPtr;
  //typedef std::map<long,long>::iterator TimeseriesIterator;

  class Webservice {
    public:
      typedef std::tr1::shared_ptr<Webservice> Ptr;
      Webservice(Config::Ptr config): _config(config) {};
      virtual ~Webservice() {};
      TimeseriesPtr get_values() throw(Flukso::GenericException);

    private:
      Webservice (const Webservice& original);
      Webservice& operator= (const Webservice& rhs);
      void run_query(std::string* buffer) throw(Flukso::CommunicationException);
      TimeseriesPtr parse_json_data(const char* inputData) 
        throw (Flukso::DataFormatException);

      Config::Ptr _config;
  };

}


#endif /* LIBFLUKSO_FLUKSO_HPP */

