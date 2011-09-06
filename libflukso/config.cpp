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

#include "config.hpp"
#include <iostream>
#include <cstdlib>
#include <popt.h>
#include <sstream>


using namespace Flukso;


Config::Ptr Config::buildConfigFromCmdLine(int argc, char const* argv[]) {
  //std::cout << "Building configuration from command line." << std::endl;
  Config::Ptr retval=Config::Ptr(new Config());
  poptContext optCon;
  int poptRC;
  static struct {
    char* token;
    char* sensor;
    char* baseURL;
    char* localip;
    char* unit;
    char* format;
    char* output;
    char* filename;
    char* interval;
    int debug;
    long int maxAge;
    long int criticalTimeDiff;
    long int warningTimeDiff;
    int mode;
    bool verbose;
  } configData;

  struct poptOption cmdLineOpts[] =
  {
    { "debug", 'd', POPT_ARG_NONE, &configData.debug, 0, "Debug mode", NULL },
    { "token", 't', POPT_ARG_STRING, &configData.token, 0, "SmartGrid token", "string" },
    { "sensor", 's', POPT_ARG_STRING, &configData.sensor, 0, "SmartGrid sensor", "string" },
    { "baseURL", 'b', POPT_ARG_STRING, &configData.baseURL, 0, "SmartGrid Base-URL", "url" },
    { "local-ip", 'l', POPT_ARG_STRING, &configData.localip, 0, "Local Flukso IP", "ip" },
    { "interval", 'i', POPT_ARG_STRING, &configData.interval, 0,
      "Time interval to fetch (hour, day, month, year, night)", "interval" },
    { "unit", 'u', POPT_ARG_STRING, &configData.unit, 0, "Unit to fetch (watt)", "unit" },
    { "format", 'f', POPT_ARG_STRING, &configData.format, 0, "Output format to use {text|xml|chumby-current|chumby-lastminute|chumby-lasthour|chumby-lastday}", "string" },
    { "output", 'o', POPT_ARG_STRING, &configData.output, 0, "Output destination: {cout|file}", "string" },
    { "filename", 'n', POPT_ARG_STRING, &configData.filename, 0, "Output filename (only use with -o file)", "string" },
    { "verbose", 'v', POPT_ARG_NONE, &configData.verbose, 0, "Verbose output", NULL },
    POPT_AUTOHELP POPT_TABLEEND // Needed to terminate array
  };

  // Get CmdLine
  optCon = poptGetContext(NULL, argc, argv, cmdLineOpts, 0);

  // Check arguemnts
  if (argc < 3) {
    poptPrintHelp(optCon, stderr, 0);
    poptFreeContext(optCon);
    exit(1);
  }

  // Parse arguments
  poptRC = poptGetNextOpt(optCon);

  // Parsing successful?
  if (poptRC < -1)
  {
    std::cerr << "Commandline Parsing failed: " 
      << poptBadOption(optCon, POPT_BADOPTION_NOALIAS) 
      << ": " <<poptStrerror(poptRC) << std::endl;
    poptFreeContext(optCon);
    exit(10);
  }

  // Check cmdline arguments for plausibility

  if (configData.debug) 
    retval->enableDebug();

  if (configData.verbose) 
    retval->enableVerbose();

  if (configData.localip != NULL) {
    // local mode
    std::ostringstream oss;
    oss << "http://" << configData.localip << "/sensor/";
    retval->setBaseurl(oss.str());
  } else if (configData.baseURL != NULL) {
    //std::cout << "baseurl: >" << configData.baseURL << "<" << std::endl;
    retval->setBaseurl(std::string(configData.baseURL));
  } else {
    if (configData.debug)
      std::cout << "Base URL omitted and no local ip specified. Using default URL https://api.mysmartgrid.de/sensor/" << std::endl;
  } 

  if (configData.token == NULL) {
    // not needed in local mode
    if (configData.localip == NULL) {
      std::cerr << std::endl << "Error: Token value MUST be provided for proper function." << std::endl << std::endl;
      poptPrintHelp(optCon, stderr, 0);
      poptFreeContext(optCon);
      exit(11);
    }
  } else {
    retval->setTokenId(std::string(configData.token));
  }

  if (configData.sensor == NULL) {
    std::cerr << std::endl << "Error: Sensor value MUST be provided for proper function." << std::endl << std::endl;
    poptPrintHelp(optCon, stderr, 0);
    poptFreeContext(optCon);
    exit(11);
  } else {
    retval->setSensorId(std::string(configData.sensor));
  }

  if (configData.unit == NULL) {
    if (configData.debug)
      std::cout << "Unit omitted. Using default unit watt" << std::endl;
  } else {
    retval->setUnit(std::string(configData.unit));
  }

  if (configData.format == NULL) {
    if (configData.debug)
      std::cout << "Format omitted. Using default format text" << std::endl;
  } else {
    retval->setFormatterType(std::string(configData.format));
  }

  if (configData.output == NULL) {
    if (configData.output)
      std::cout << "Output filter omitted. Using default output filter cout." << std::endl;
  } else {
    retval->setFilterType(std::string(configData.output));
  }

  if (configData.filename != NULL) {
    retval->setOutputFilename(std::string(configData.filename));
  }


  if (configData.interval == NULL) {
    if (configData.debug)
      std::cout << "Intervall omitted. Setting default to hour" << std::endl;
  } else {
    retval->setTimeInterval(std::string(configData.interval));
  }

  if (retval->verbose()) {
    retval->printConfig();
  }

  poptFreeContext(optCon);
  return retval;
}



void Config::printConfig() {
  std::cout << "Using configuration:" << std::endl;
  std::cout << " - base url: " << getBaseurl() << std::endl;
  std::cout << " - sensor id: " << getSensorId() << std::endl;
  std::cout << " - token id: " << getTokenId() << std::endl;
  std::cout << " - unit: " << getUnit() << std::endl;
  std::cout << " - time interval: " << getTimeInterval() << std::endl;
  std::cout << " - format: " << getFormatterType() << std::endl;
}

// getter methods

const std::string& Config::getBaseurl() { return _baseurl; }
const std::string& Config::getSensorId() { return _sensor; }
const std::string& Config::getTokenId() { return _token; }
const std::string& Config::getUnit() { return _unit; }
const std::string& Config::getTimeInterval() { return _interval; }
const std::string& Config::getFormatterType() { return _formattertype; }
const std::string& Config::getFilterType() { return _filtertype; };
const std::string& Config::getOutputFilename() { return _outfilename;};
bool Config::debug() { return _debug; }
bool Config::verbose() { return _verbose; }

// setter methods
void Config::setBaseurl(const std::string& baseurl) {
  _baseurl = baseurl;
}
void Config::setSensorId(const std::string& sensor) {
  _sensor = sensor;
}
void Config::setTokenId(const std::string& token) {
  _token = token;
}
void Config::setUnit(const std::string& unit) {
  _unit = unit;
}
void Config::setTimeInterval(const std::string& interval) {
  _interval = interval;
}
void Config::setFormatterType(const std::string& formatter) {
  _formattertype = formatter;
}
void Config::setFilterType(const std::string& filter) {
  _filtertype = filter;
}
void Config::setOutputFilename(const std::string& filename) {
  _outfilename = filename;
}
void Config::enableDebug() {
  _debug = true;
}
void Config::enableVerbose() {
  _verbose = true;
}

