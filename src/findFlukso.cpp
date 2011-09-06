/**
 * Project: findFlukso: howl wrapper for the chumby
 *   
 * @author Stephan Platz, Project mySmartGrid https://www.mysmartgrid.de, Copyright (C) 2010.
 *   
 * @see The GNU Public License (GPL)
 */
/* 
 * This program is free software; you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation; either version 3 of the License, or 
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
 * for more details.
 * 
 * You should have received a copy of the GNU General Public License along 
 * with this program; if not, write to the Free Software Foundation, Inc., 
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

extern "C" {
	#include <howl.h>
	#include <salt/debug.h>
}
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "config.hpp"
#include "flukso.hpp"
#include "formatter.hpp"
#include "outputfilter.hpp"

#define DEBUG

std::map<std::string, std::string> ip_map;
Flukso::Config::Ptr config;

static sw_result HOWL_API
add_resolver(
				sw_discovery		discovery,
				sw_discovery_oid	oid,
				sw_uint32		interface_index,
				sw_const_string		name,
				sw_const_string		type,
				sw_const_string		domain,
				sw_ipv4_address		address,
				sw_port			port,
				sw_octets		text_record,
				sw_uint32		text_record_len,
				sw_opaque_t		extra)
{
	sw_text_record_iterator				it;
	sw_int8						name_buf[16];
	//sw_uint8					oval[SW_TEXT_RECORD_MAX_LEN];
	//sw_uint32					oval_len;
	sw_result					err = SW_OKAY;

	sw_discovery_cancel(discovery, oid);
	
	std::stringstream ip_address;
	ip_address << sw_ipv4_address_name(address, (char*) name_buf, 16);
	ip_address << ":";
	ip_address << port;
	ip_map[ip_address.str()] = (std::string) name;

	#ifdef DEBUG
		if (sw_text_record_iterator_init(&it, text_record, text_record_len) !=- SW_OKAY)
		{
			fprintf(stderr, "init failed\n");
			return err;
		}

		char						key[SW_TEXT_RECORD_MAX_LEN];
		sw_octet					val[SW_TEXT_RECORD_MAX_LEN];
		sw_ulong					val_len;

		std::string id, path, version = "";
		std::stringstream temp;

		while (sw_text_record_iterator_next(it, key, val, &val_len) == SW_OKAY && key != "")
		{
			std::string keystring = std::string(key);
			if ( keystring.compare("id1") == 0 )
			{
				temp.str("");
				temp << val;
				id = temp.str();
				std::cout << "id1: " << id << std::endl;
			}
			else if ( keystring.compare("id2") == 0 && id.length() == 0 )
			{
				temp.str("");
				temp << val;
				id = temp.str();
				std::cout << "id2: " << id << std::endl;
			}
			else if ( keystring.compare("path") == 0 )
			{
				temp.str("");
				temp << val;
				path = temp.str();
				std::cout << "path: " << path << std::endl;
			}
			else if ( keystring.compare("version") == 0 )
			{
				temp.str("");
				temp << val;
				version = temp.str();
				std::cout << "version: " << val << std::endl;
			}
			else
				std::cout << "txt-record found: '" << key << "'=" << val << std::endl;
		}

		if ( id.length() > 0 && path.length() > 0 && version.length() > 0 )
		{
			std::cout << "url: http://" << ip_address.str() << path << "/" << id << "?interval=minute&unit=watt&version=" << version << std::endl;
			config = Flukso::Config::Ptr(new Flukso::Config());
			config->setBaseurl("http://"+ip_address.str()+path+"/");
			config->setSensorId(id);
			config->setTimeInterval("minute");
			config->setFormatterType("chumby-current");
			config->setFilterType("file");
			config->setOutputFilename("/tmp/flukso/last_reading");
			#ifdef DEBUG
				config->enableVerbose();
			#endif
		}

		sw_text_record_iterator_fina(it);
	#endif

	#ifdef DEBUG
	std::cout << name << "(" << ip_address.str() << ") added." << std::endl;
	#endif

	return err;
}

static sw_result HOWL_API
rem_resolver(
				sw_discovery		discovery,
				sw_discovery_oid	oid,
				sw_uint32		interface_index,
				sw_const_string		name,
				sw_const_string		type,
				sw_const_string		domain,
				sw_ipv4_address		address,
				sw_port			port,
				sw_octets		text_record,
				sw_uint32		text_record_len,
				sw_opaque_t		extra)
{
	//sw_text_record_iterator				it;
	sw_int8						name_buf[16];
	//sw_int8						key[SW_TEXT_RECORD_MAX_LEN];
	//sw_int8						sval[SW_TEXT_RECORD_MAX_LEN];
	//sw_uint8					oval[SW_TEXT_RECORD_MAX_LEN];
	//sw_uint32					oval_len;
	sw_result					err = SW_OKAY;

	sw_discovery_cancel(discovery, oid);
	
	std::string ip_address = (std::string) sw_ipv4_address_name(address, (char*) name_buf, 16);
	ip_map.erase(ip_address);

	#ifdef DEBUG
	std::cout << name << "(" << ip_address << ") removed." << std::endl;
	#endif

	return err;
}

static sw_result HOWL_API
my_browser(
				sw_discovery				discovery,
				sw_discovery_oid			oid,
				sw_discovery_browse_status		status,
				sw_uint32				interface_index,
				sw_const_string				name,
				sw_const_string				type,
				sw_const_string				domain,
				sw_opaque_t				extra)
{
	sw_discovery_resolve_id rid;

	#ifdef DEBUG
		switch (status)
		{
			case SW_DISCOVERY_BROWSE_INVALID:
				std::cout << "result: browse invalid" << std::endl;
				break;
			case SW_DISCOVERY_BROWSE_RELEASE:
				std::cout << "result: browse release" << std::endl;
				break;
			case SW_DISCOVERY_BROWSE_ADD_DOMAIN:
				std::cout << "result: browse add domain (" << domain << ")" << std::endl;
				break;
			case SW_DISCOVERY_BROWSE_ADD_DEFAULT_DOMAIN:
				std::cout << "result: browse add default domain (" << domain << ")" << std::endl;
				break;
			case SW_DISCOVERY_BROWSE_REMOVE_DOMAIN:
				std::cout << "result: browse remove domain (" << domain << ")" << std::endl;
				break;
			case SW_DISCOVERY_BROWSE_ADD_SERVICE:
				std::cout << "result: browse add service (" << name << "[" << type << "])" << std::endl;
				break;
			case SW_DISCOVERY_BROWSE_REMOVE_SERVICE:
				std::cout << "result: browse remove service (" << name << "[" << type << "])" << std::endl;
				break;
		}
	#endif

	if (status == SW_DISCOVERY_BROWSE_ADD_SERVICE)
	{
		if (sw_discovery_resolve(discovery, interface_index, name, type, domain, add_resolver, extra, &rid) != SW_OKAY)
		{
			std::cerr << "resolve failed" << std::endl;
		}
	}
	else if (status == SW_DISCOVERY_BROWSE_REMOVE_SERVICE)
	{
		if (sw_discovery_resolve(discovery, interface_index, name, type, domain, rem_resolver, extra, &rid) != SW_OKAY)
		{
			std::cerr << "resolve failed" << std::endl;
		}
	}

	return SW_OKAY;
}

int findFlukso(
		char* type,
		char* filename)
{
	sw_discovery		discovery;
	sw_discovery_oid	oid;
	sw_result			err;
	bool run = true;

	sw_ulong timeout = 5000; //in msecs

	err = sw_discovery_init(&discovery);
	sw_check_okay(err, exit);

	sw_salt salt;
	err = sw_discovery_salt(discovery, &salt);
	sw_check_okay(err, exit);

	err = sw_discovery_browse(discovery, 0, type, NULL, my_browser, NULL, &oid);
	sw_check_okay(err, exit);

	while (run)
	{
		sw_salt_step(salt, &timeout);

		std::ofstream file;
		file.open(filename);
		#ifdef DEBUG
			std::cout << "Map size: " << ip_map.size() << std::endl;
		#endif
		for (std::map<std::string, std::string>::iterator ii=ip_map.begin(); ii!=ip_map.end(); ++ii)
		{
			std::cout << (*ii).first << ":" << (*ii).second << std::endl;
			file << (*ii).first << std::endl;
		}
		file.close();

		if (config)
		{
			try {
				Flukso::Webservice::Ptr webservice(new Flukso::Webservice(config));
				Flukso::TimeseriesPtr ts(webservice->get_values());
				Flukso::Formatter::Ptr formatter = Flukso::Formatter::buildFormatter(config, ts);
				Flukso::OutputFilter::Ptr filter = Flukso::OutputFilter::buildFilter(config);
				filter->render(formatter);
			} catch (Flukso::GenericException ge) {
				std::cout << "Failed to retrieve values, reason:" << std::endl 
					<< "  " << ge.reason() << std::endl;
				//return -1;
			}	catch (std::exception e) {
				std::cout << "Unknown exception occured, reason:" << std::endl
					<< "  " << e.what() << std::endl;
				//return -2;
			}
		}
	}

	return 0;
exit:

	return 1;
}

int main(
	int		argc,
	char	**	argv)
{
	if (argc == 3)
	{
		return findFlukso(argv[1], argv[2]);
	}
	else
	{
		std::cout << "Usage: findFlukso [service] [file]" << std::endl;
		return 1;
	}
}
