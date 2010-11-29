extern "C" {
	#include <howl.h>
	#include <salt/debug.h>
}
#include <map>
#include <iostream>
#include <fstream>
#include <string>

#define DEBUG

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

	#ifdef DEBUG
	std::cout << name << "(" << ip_address << ") added." << std::endl;
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

	std::cout << "1" << std::endl;

	sw_ulong timeout = 5000; //in msecs

	err = sw_discovery_init(&discovery);
	sw_check_okay(err, exit);

	std::cout << "2" << std::endl;

	sw_salt salt;
	err = sw_discovery_salt(discovery, &salt);
	sw_check_okay(err, exit);

	std::cout << "3" << std::endl;

	err = sw_discovery_browse(discovery, 0, type, NULL, my_browser, NULL, &oid);
	sw_check_okay(err, exit);

	std::cout << "4" << std::endl;

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
			#ifdef DEBUG
				std::cout << (*ii).first << ":" << (*ii).second << std::endl;
			#endif
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
