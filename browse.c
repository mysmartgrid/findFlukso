/*
 * Copyright 2003, 2004, 2004 Porchdog Software. All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without modification,
 *	are permitted provided that the following conditions are met:
 *
 *		1. Redistributions of source code must retain the above copyright notice,
 *		   this list of conditions and the following disclaimer.   
 *		2. Redistributions in binary form must reproduce the above copyright notice,
 *		   this list of conditions and the following disclaimer in the documentation
 *		   and/or other materials provided with the distribution.
 *
 *	THIS SOFTWARE IS PROVIDED BY PORCHDOG SOFTWARE ``AS IS'' AND ANY
 *	EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *	IN NO EVENT SHALL THE HOWL PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 *	INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 *	OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *	OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 *	OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *	The views and conclusions contained in the software and documentation are those
 *	of the authors and should not be interpreted as representing official policies,
 *	either expressed or implied, of Porchdog Software.
 */

#include <howl.h>
#include <salt/debug.h>
#include <stdio.h>
#include <stdbool.h>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <pthread.h>
#include <string.h>

#define DEBUG

extern "C" {
	static char* list;

	static sw_result HOWL_API
	my_resolver(
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
					sw_opaque_t		extra);

	static sw_result HOWL_API
	my_browser(
					sw_discovery				discovery,
					sw_discovery_oid			oid,
					sw_discovery_browse_status		status,
					sw_uint32				interface_index,
					sw_const_string				name,
					sw_const_string				type,
					sw_const_string				domain,
					sw_opaque_t				extra);

	int findFlukso(
			char* type,
			char* filename);
}

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
	sw_text_record_iterator				it;
	sw_int8						name_buf[16];
	sw_int8						key[SW_TEXT_RECORD_MAX_LEN];
	sw_int8						sval[SW_TEXT_RECORD_MAX_LEN];
	sw_uint8					oval[SW_TEXT_RECORD_MAX_LEN];
	sw_uint32					oval_len;
	sw_result					err = SW_OKAY;

	sw_discovery_cancel(discovery, oid);
	
	std::string ip_address = (std::string) sw_ipv4_address_name(address, (char*) name_buf, 16);
	ip_map[ip_address] = (std::string) name;

	#ifdef DEBUG
	std::cout << name << "(" << ip_address << ") added." << std::endl;
	#endif

exit:

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
	sw_text_record_iterator				it;
	sw_int8						name_buf[16];
	sw_int8						key[SW_TEXT_RECORD_MAX_LEN];
	sw_int8						sval[SW_TEXT_RECORD_MAX_LEN];
	sw_uint8					oval[SW_TEXT_RECORD_MAX_LEN];
	sw_uint32					oval_len;
	sw_result					err = SW_OKAY;

	sw_discovery_cancel(discovery, oid);
	
	std::string ip_address = (std::string) sw_ipv4_address_name(address, (char*) name_buf, 16);
	ip_map.erase(ip_address);

	#ifdef DEBUG
	std::cout << name << "(" << ip_address << ") removed." << std::endl;
	#endif

exit:

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
			fprintf(stderr, "resolve failed\n");
		}
	}
	else if (status == SW_DISCOVERY_BROWSE_REMOVE_SERVICE)
	{
		if (sw_discovery_resolve(discovery, interface_index, name, type, domain, rem_resolver, extra, &rid) != SW_OKAY)
		{
			fprintf(stderr, "resolve failed\n");
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

	sw_ulong timeout = 10000; //in msecs

	/*pthread_t tid;*/
	/*int result = pthread_create(&tid, NULL, thread_run, &discovery);*/
	/*printf("RESULT: %d", result);*/

	/*if ( result != 0 )*/
		/*run = false;*/

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

		/*FILE *file;*/
		/*file = fopen(filename, "w");*/
		/*if(file)*/
		/*{*/
			/*fprintf(file, list);*/
			/*printf(list);*/
			std::ofstream file;
			file.open(filename);
			std::cout << "Map size: " << ip_map.size() << std::endl;
			for (std::map<std::string, std::string>::iterator ii=ip_map.begin(); ii!=ip_map.end(); ++ii)
			{
				#ifdef DEBUG
					std::cout << (*ii).first << ":" << (*ii).second << std::endl;
				#endif
				file << (*ii).first << ":" << (*ii).second << std::endl;
			}
			file.close();
			/*free(list);*/
			/*fclose(file);*/
		/*}*/
	}

	return 0;
exit:

	return 1;
}

main(
	int		argc,
	char	**	argv)
{
	//fprintf(stderr, "[DEBUG] argc: %d\n", argc);
	if (argc == 3)
	{
		return findFlukso(argv[1], argv[2]);
	}
	else
	{
		printf("Usage: findFlukso [service] [file]\n");
	}

exit:

		return 1;
}
