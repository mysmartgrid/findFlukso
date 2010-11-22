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
#include <pthread.h>
#include <string.h>

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
	
	char* ip_address = (char*) sw_ipv4_address_name(address, name_buf, 16);
	//printf("[DEBUG] ip_address: %s\n", ip_address);

	//printf("[DEBUG] Name: %s\n", name);
	//printf("[DEBUG] strlen: %d - %d\n", strlen(list), strlen(name));

	char* new_list = (char*) malloc(strlen(list)+strlen(name)+18);
	strcpy(new_list, list);
	int length = strlen(list);

	//printf("[DEBUG] newlist: %u; %s\n", new_list, new_list);
	strcpy(new_list + length, name);

	//printf("[DEBUG] new_list: %s\n", new_list);
	length += strlen(name);
	strcpy(new_list + length, "\t");

	//printf("[DEBUG] new_list: %s\n", new_list);
	length += 1;
	strcpy(new_list + length, ip_address);

	//printf("[DEBUG] new_list: %s\n", new_list);
	length += strlen(ip_address);
	strcpy(new_list + length, "\n");

	free(list);
	
	list = new_list;
	//printf("[DEBUG] list: %u; %s\n", list, list);
	//printf("Device found: %s", list);

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

	if (status == SW_DISCOVERY_BROWSE_ADD_SERVICE)
	{
		if (sw_discovery_resolve(discovery, interface_index, name, type, domain, my_resolver, extra, &rid) != SW_OKAY)
		{
			fprintf(stderr, "resolve failed\n");
		}
	}
	else if (status == SW_DISCOVERY_BROWSE_REMOVE_SERVICE)
	{
		printf("%s removed\n", name);
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

	sw_ulong timeout = 1000; //in msecs

	while (run)
	{
		list = (char*) malloc(1);
		strcpy(list, "");


		/*err = sw_discovery_run(discovery);*/
		/*sw_check_okay(err, exit);*/

		//printf("[DEBUG] list: %s", list, list);

		sw_salt_step(salt, &timeout);

		FILE *file;
		file = fopen(filename, "w");
		if(!file)
			break;
		fprintf(file, list);
		printf(list);
		free(list);
		fclose(file);
	}

	return 0;
exit:

	return 1;
}

#if defined(WIN32)
int __cdecl
#else
int
#endif
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
