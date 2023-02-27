#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <string>
# include <sstream>

# include "parse_utils.hpp"
# include "ServerLocation.hpp"

class ServerConfig
{
private:
	ServerConfig();

public:
	typedef const ServerLocation constServerLocation;

private:
	//	Attribute
	std::stringstream&					content;
	std::string							root;
	std::string							server_name;
	std::pair<int, short>				listen_port;
	std::map<int, std::string>			error_page;
	std::vector<constServerLocation>	locations;

public:
	// Member functions
	//		Constructor
	ServerConfig(std::stringstream& config);
	ServerConfig(const ServerConfig& other);
	//Assignment operator
	ServerConfig&	operator=(const ServerConfig& other);
	//		Destructor
	~ServerConfig();

	//	Location finder
	// std::string	find_location(std::string path);

	// Getters
	int						get_ip() const;
	short					get_port() const;
	const std::string&		get_server_name() const;
	const std::string&		get_root() const;
	const std::string&		get_target(std::string init_target) const;

private:
	//	Parsing functions
	void						_parse();
	void						_parse_line(std::string& line);
	void						_insert_token(std::vector<std::string> tokens);
	std::pair<int, short>		_parse_address(std::string& address);
	void						_add_location(std::vector<std::string>& tokens);
	const ServerLocation&		_get_location(std::string target) const;
};

#endif