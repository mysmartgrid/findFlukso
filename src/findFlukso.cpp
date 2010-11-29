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
#include <string>

bool debug = false;

std::map<std::string, std::string> ip_map;

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
	//sw_text_record_iterator				it;
	sw_int8						name_buf[16];
	//sw_int8						key[SW_TEXT_RECORD_MAX_LEN];
	//sw_int8						sval[SW_TEXT_RECORD_MAX_LEN];
	//sw_uint8					oval[SW_TEXT_RECORD_MAX_LEN];
	//sw_uint32					oval_len;
	sw_result					err = SW_OKAY;

	sw_discovery_cancel(discovery, oid);
	
	std::string ip_address = (std::string) sw_ipv4_address_name(address, (char*) name_buf, 16);
	ip_map[ip_address] = (std::string) name;

	if (debug)
		std::cout << name << "(" << ip_address << ") added." << std::endl;

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

	if (debug)
		std::cout << name << "(" << ip_address << ") removed." << std::endl;

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

	if (debug)
	{
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
	}

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
		char* filename,
		sw_ulong timeout)
{
	sw_discovery		discovery;
	sw_discovery_oid	oid;
	sw_result			err;
	bool run = true;

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
		if (debug)
			std::cout << "Map size: " << ip_map.size() << std::endl;

		for (std::map<std::string, std::string>::iterator ii=ip_map.begin(); ii!=ip_map.end(); ++ii)
		{
			if (debug)
				std::cout << (*ii).first << ":" << (*ii).second << std::endl;
			file << (*ii).first << ":" << (*ii).second << std::endl;
		}
		file.close();
	}

	return 0;
exit:

	return 1;
}

int main(
	int		argc,
	char	**	argv)
{
	int timeout = 5000;
	int result = 1;

	if (argc == 2 && !strcmp(argv[1], "-h"))
	{
		std::cout << "Usage: " << argv[0] << " [service] [file] ([timeout])" << std::endl;
		std::cout << std::endl;
		std::cout << "Specific options:" << std::endl;
		std::cout << "\t service \t service string published by the device (e.g. _flukso-realtime._tcp)" << std::endl;
		std::cout << "\t file \t\t file to write the ip addresses of found devices to (e.g.: /tmp/flukso)" << std::endl;
		std::cout << "\t timeout \t time interval between writing to file in milliseconds. If set to 0 the program only runs once. (default: 5000)" << std::endl;
		return 0;
	}

	if (argc > 1 && !strcmp(argv[1], "-d"))
	{
		debug = true;
		switch (argc)
		{
			case 5:
				timeout = atoi(argv[4]);
				if (debug)
					std::cout << "Timeout: " << timeout << std::endl;
			case 4:
				result = findFlukso(argv[2], argv[3], timeout);
				break;
			default:
				std::cout << "Usage: " << argv[0] << " [service] [file] ([timeout]) (" << argv[0] << " -h for more information)" << std::endl;
				break;
		}
	}
	else
	{
		switch (argc)
		{
			case 4:
				timeout = atoi(argv[3]);
				if (debug)
					std::cout << "Timeout: " << timeout << std::endl;
			case 3:
				result = findFlukso(argv[1], argv[2], timeout);
				break;
			default:
				std::cout << "Usage: " << argv[0] << " [service] [file] ([timeout]) (" << argv[0] << " -h for more information)" << std::endl;
				break;
		}
	}
	return result;
}
