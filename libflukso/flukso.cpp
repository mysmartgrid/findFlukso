/**
 * This file is part of libflukso.
 *
 * (c) Henning Bitsch <henning@bitsch.de>, 2010
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


#include "flukso.hpp"
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <vector>
#include <json/json.h>

using namespace Flukso;


/**
 * Callable from the outside - triggers HTTP retrieval and decoding 
 * of data.
 */
TimeseriesPtr Webservice::get_values() throw (Flukso::GenericException) {
  std::string buffer;
  run_query(&buffer);
  TimeseriesPtr retval = parse_json_data(buffer.c_str());
  buffer.clear();
  return retval;
}

// This is the writer call back function used by curl
int writer(char *data, size_t size, size_t nmemb,
                  std::string *localbuffer) {
  // What we will return
  int result = 0;
  // Is there anything in the buffer?
  if (localbuffer != NULL)
  {
    // Append the data to the buffer
    localbuffer->append(data, size * nmemb);
    // How much did we write?
    result = size * nmemb;
  }
  return result;
}

/**
 * Parse the JSON data and create a proper timeseries datastructure.
 */
TimeseriesPtr Webservice::parse_json_data(const char* inputData) throw (Flukso::DataFormatException) {
  struct json_object *myJSONobj;
  struct json_object *myCurrent;
  struct json_object *myCurrentElement;
  unsigned int length;
  TimeseriesPtr ts(new Timeseries());

  myJSONobj = json_tokener_parse(inputData);
  length = json_object_array_length(myJSONobj);

  if (_config->debug())
    std::cout << "Data length: " << length << " bytes" << std::endl;


  // Continue if we have elements
  if (length > 0) {
    // Walk through all entries
    for (unsigned int i = 0; i < length; i++) {
      // Get an entries
      myCurrent = json_object_array_get_idx(myJSONobj, i);
      // Is the current entry an array?
      if (json_object_get_type(myCurrent) != json_type_array) {
        throw Flukso::DataFormatException("Error decoding Flukso data - no value array found.");
      }

      // Has it two elements?
      if (json_object_array_length(myCurrent) != 2) {
        std::cerr << "Array element malformed. Expected two elements but got " 
          << json_object_array_length(myCurrent) << ": "
          << json_object_get_string(myCurrent) << std::endl;
        continue;
      }

      //OK, start your engine
      long timestamp, value = 0;

      // get timestamp.
      myCurrentElement = json_object_array_get_idx(myCurrent, 0);
      if (json_object_get_type(myCurrentElement) != json_type_int) {
        std::cerr << "First array element is not an integer at entry" << i << ": " 
          << json_object_get_string(myCurrent) << std::endl;
        continue;
      }
      timestamp=json_object_get_int(myCurrentElement);

      // get value associated with the current timestamp.
      myCurrentElement = json_object_array_get_idx(myCurrent, 1);
      if (json_object_get_type(myCurrentElement) == json_type_int) {
        value = json_object_get_int(myCurrentElement);
      } else {
        std::cerr << "Second array element is neither string or integer at entry" << i
          << ": " << json_object_get_string(myCurrent) << std::endl;
        continue;
      }
      ts->insert(std::pair<long,long>(timestamp, value));
      //json_object_put(myCurrentElement);
    }
  } else {
    // free json memory.
    json_object_put(myJSONobj);
    throw Flukso::DataFormatException("Tokenizing JSON response failed." );
  }
  // free json memory.
  json_object_put(myJSONobj);
  return ts;
}

/**
 * Wraps CURL in one function call.
 */
void Webservice::run_query(std::string* buffer) throw (Flukso::CommunicationException){
  CURL *curl;
  CURLcode result;
  char errorBuffer[CURL_ERROR_SIZE];
  struct curl_slist *slist = NULL;
  long http_code = 0;

  std::ostringstream oss;
  oss << _config->getBaseurl() << _config->getSensorId()
    << "?interval=" << _config->getTimeInterval() 
    << "&unit=" << _config->getUnit();
  std::string url(oss.str());
  if (_config->verbose()) 
    std::cout << "Query url: " << url << std::endl;

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    // Disable Peer Verification
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);  
    // Adding Headers
    slist = curl_slist_append(slist, "Accept: application/json");
    slist = curl_slist_append(slist, "X-Version: 1.0");
    std::ostringstream oss2;
    oss2 << "X-Token: " << _config->getTokenId();
    std::string token_header(oss2.str());
    //std::cout << "Token header: " << token_header << std::endl;
    slist = curl_slist_append(slist, token_header.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);

    // Forward received data to own function
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);

    // Performing request
    if (_config->debug())
      std::cout << "Connecting to server and fetching data ...";
    result = curl_easy_perform(curl);
    if (_config->debug())
      std::cout << "finished." << std::endl;

    curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
    if (_config->verbose()) {
      std::cout << "HTTP response code: " << http_code << std::endl;
    }
    if (http_code == 200 && result != CURLE_ABORTED_BY_CALLBACK) {
      if (_config->verbose()) {
        std::cout << "HTTP connection succeeded" << std::endl;
        std::cout << "Server data:" << std::endl;
        std::cout << *buffer << std::endl;
      }
    } else {
      // Communication problem.
      if (_config->verbose()) {
        std::cout << "HTTP failed: [CURL: " << result << ", " 
          << curl_easy_strerror(result) << "] - " << errorBuffer;
        std::cout << "Response body:" << std::endl;
        std::cout << *buffer << std::endl;
      }
      // Cleaning up
      curl_easy_cleanup(curl);
      curl_slist_free_all(slist);
      // Raise a proper exception
      std::ostringstream eoss;
      eoss << "Communication error - HTTP response: " << http_code 
        << ", CURL error " << result << ": " << curl_easy_strerror(result);
      throw Flukso::CommunicationException(eoss.str());
    }
  }
  // Cleaning up
  curl_easy_cleanup(curl);
  curl_slist_free_all(slist);
}
