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

#ifndef LIBFLUKSO_CONFIG_HPP
#define LIBFLUKSO_CONFIG_HPP 1

#include <common.hpp>
#include <string>


namespace Flukso {
  class Config {
	public:
	  typedef std::tr1::shared_ptr<Config> Ptr;
	  virtual ~Config(){};
	  static Config::Ptr buildConfigFromCmdLine(const int argc, char const* argv[]);
	  void printConfig();
	  Config():
		_baseurl("https://api.mysmartgrid.de/sensor/"), _sensor("undefined"), 
		_token("undefined"), _unit("watt"), _interval("hour"),  _formattertype("text"), 
		_filtertype("cout"), _outfilename("fluksovalues.txt"), _debug(false),
		_verbose(false) {};
	  Config (const Config& original);
	  const std::string& getBaseurl();
	  const std::string& getSensorId();
	  const std::string& getTokenId();
	  const std::string& getUnit();
	  const std::string& getTimeInterval();
	  const std::string& getFormatterType();
	  const std::string& getFilterType();
	  const std::string& getOutputFilename();
	  bool debug();
	  bool verbose();
	  void setBaseurl(const std::string& baseurl);
	  void setSensorId(const std::string& sensor);
	  void setTokenId(const std::string& token);
	  void setUnit(const std::string& unit);
	  void setTimeInterval(const std::string& interval);
	  void setFormatterType(const std::string& formatter);
	  void setFilterType(const std::string& filter);
	  void setOutputFilename(const std::string& filename);
	  void enableDebug();
	  void enableVerbose();


	private:
	  Config& operator= (const Config& rhs);
	  std::string _baseurl;
	  std::string _sensor;
	  std::string _token;
	  std::string _unit;
	  std::string _interval;
	  std::string _formattertype;
	  std::string _filtertype;
	  std::string _outfilename;
	  bool _debug;
	  bool _verbose;

  };

}


#endif /* LIBFLUKSO_CONFIG_HPP */

