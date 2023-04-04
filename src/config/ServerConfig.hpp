#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include "unistd.h"

# include <string>
# include <sstream>
# include <set>

# include "parse_utils.hpp"
# include "ServerLocation.hpp"
# include "webserv.hpp"

class ServerConfig
{
private:
	//	Attribute
	std::stringstream*					content;
	std::string							root;
	std::string							server_name;
	std::string							index;
	std::pair<int, short>				listen_port;
	std::map<int, std::string>			error_page;
	std::vector<ServerLocation>			locations;
	string_map							cgi;

	//	Constructors
	ServerConfig();
	//	Assignment operator
	ServerConfig&	operator=(const ServerConfig& other);
public:
	// Member functions
	//		Constructor
	ServerConfig(std::stringstream* config);
	ServerConfig(const ServerConfig& other);
	//		Destructor
	~ServerConfig();

	//	Location finder
	// std::string	find_location(std::string path);

	// Getters
	int									get_ip() const;
	short								get_port() const;
	const std::string&					get_server_name() const;
	const std::string&					get_root() const;
	const string_map&					get_cgi() const;
	const std::map<int, std::string>&	get_error_page() const;
	const ServerLocation*				get_location_addr(std::string target) const;

	// Check
	void					check() const;

private:
	//	Parsing functions
	void						_parse();
	void						_parse_line(std::string& line);
	void						_insert_token(std::vector<std::string> tokens);
	std::pair<int, short>		_parse_address(std::string& address);
	void						_add_location(std::vector<std::string>& tokens);
};

#endif