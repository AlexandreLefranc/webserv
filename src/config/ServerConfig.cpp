#include "ServerConfig.hpp"

/*==============================================================================

							PUBLIC MEMBER FUNCTIONS.

==============================================================================*/

/*==============================================================================
	Constructors.
==============================================================================*/

// ServerConfig::ServerConfig()
// {
// 	return ;
// }

ServerConfig::ServerConfig(std::stringstream* config)
	: content(config)
	, index("index.html")
	, listen_port(std::make_pair(0, 80))
{
	std::cout << RED << "[ServerConfig] Initiate Config" << CRESET << std::endl;
	_parse();
	if (root.empty())
		throw (ParsingException());
}

ServerConfig::ServerConfig(const ServerConfig& other)
	: content(other.content)
	, root(other.root)
	, server_name(other.server_name)
	, listen_port(other.listen_port)
	, error_page(other.error_page)
	, locations(other.locations)
	, cgi(other.cgi)
{
	return ;
}

/*==============================================================================
	Destructors.
==============================================================================*/

ServerConfig::~ServerConfig()
{
	return ;
}

/*==============================================================================
	Assignment operator.
==============================================================================*/

// ServerConfig&	ServerConfig::operator=(const ServerConfig& other)
// {
// 	if (this != &other)
// 	{
// 		// content = other.content;
// 		root = other.root;
// 		server_name = other.server_name;
// 		listen_port = other.listen_port;
// 		error_page = other.error_page;
// 		locations = other.locations;
// 	}
// 	return (*this);
// }

/*==============================================================================
	Getters.
==============================================================================*/

int		ServerConfig::get_ip() const
{
	return (listen_port.first);
}

short	ServerConfig::get_port() const
{
	return (listen_port.second);
}

const std::string&	ServerConfig::get_server_name() const
{
	return (server_name);
}

const std::string&	ServerConfig::get_root() const
{
	return (root);
}

const string_pair&	ServerConfig::get_cgi() const
{
	return (cgi);
}

const std::map<int, std::string>&	ServerConfig::get_error_page() const
{
	return (error_page);
}

const ServerLocation*	ServerConfig::get_location_addr(std::string target) const
{
	std::vector<ServerLocation>::const_iterator	it = locations.begin();

	while (it != locations.end())
	{
		if (it->location_is_match(target))
			return (it.base()) ;
		it++;
	}
	throw (ResponseException());
}


/*==============================================================================
	Check.
==============================================================================*/

void	ServerConfig::check() const
{
	std::map<int, std::string>::const_iterator it1;
	for (it1 = error_page.begin(); it1 != error_page.end(); ++it1)
	{
		if (access(it1->second.c_str(), R_OK) != 0)
		{
			std::cout	<< BRED << "Config check failed: "
						<< it1->second << " does not exist or is not readable" << CRESET << std::endl;
			throw ParsingException();
		}
	}

	if (locations.empty() == true)
	{
		std::cout	<< BRED << "Config check failed: No location" << CRESET << std::endl;
		throw ParsingException();
	}

	std::vector<ServerLocation>::const_iterator it2;
	for (it2 = locations.begin(); it2 != locations.end(); ++it2)
		it2->check();

	if (cgi.second.empty() == false)
	{
		if (access(cgi.second.c_str(), X_OK) != 0)
		{
			std::cout	<< BRED << "Config check failed: "
						<< cgi.second << " does not exist or is not executable" << CRESET << std::endl;
			throw ParsingException();
		}
	}
}

/*==============================================================================

							PRIVATE MEMBER FUNCTIONS.

==============================================================================*/

/*==============================================================================
	Parsing functions.
==============================================================================*/

void	ServerConfig::_parse()
{
	std::string		line;

	while (std::getline(*content, line))
	{
		std::cout << line << std::endl;
		line = format_line(line);
		std::cout << RED << "[Server]Config line: " << line << CRESET << std::endl;
		if (line.empty())
			continue ;
		else if (line == "}")
			return ;
		_parse_line(line);
	}
}

void	ServerConfig::_parse_line(std::string& line)
{
	std::vector<std::string>	tokens;

	if (line.find("{") != std::string::npos)
	{
		tokens = split_tokens(line);
		if (tokens.front() == "location" && tokens.size() <= 4 && !root.empty())
			_add_location(tokens);
		else
			throw (ParsingException());
	}
	else if (line == "}")
			return ;
	else
	{
		tokens = split_tokens(line);
		if (tokens.size() == 0)
			throw (ParsingException());
		// std::cout << "Splitted token length: " << tokens.size() << std::endl	;
		_insert_token(tokens);
	}
	return ;
}

void	ServerConfig::_insert_token(std::vector<std::string> tokens)
{
	if (tokens.front() == "root" && tokens.size() == 2 && root.empty())
	{
		root = tokens[1];
		std::cout << RED << "[Server]Got root: " << root << CRESET << std::endl;
	}
	else if (tokens.front() == "server_name" && tokens.size() == 2 && server_name.empty())
	{
		server_name = tokens[1];
		std::cout << RED << "[Server]Got server name: " << server_name << CRESET << std::endl;
	}
	else if (tokens.front() == "listen" && tokens.size() == 2 && listen_port == std::make_pair<int, short>(0, 80))
	{
		listen_port = _parse_address(tokens[1]);
		std::cout << RED << "[Server]Got port: " << listen_port.first << ":";
		std::cout << listen_port.second << CRESET << std::endl;
	}
	else if (tokens.front() == "error_page" && tokens.size() == 3)
	{
		error_page.insert(std::make_pair(std::atoi(tokens[1].c_str()), tokens[2]));
		std::cout << RED << "[Server]Got error page: " << tokens[1] << " -> ";
		std::cout << tokens[2] << CRESET << std::endl;
	}
	else if (tokens.front() == "index" && tokens.size() == 2 && index == "index.html")
	{
		index = tokens[1];
		std::cout << RED << "[Server]Got index: " << index << CRESET << std::endl;
	}
	else if (tokens.front() == "cgi" && tokens.size() == 3 && cgi.first.empty())
	{
		cgi = std::make_pair(tokens[1], tokens[2]);
		std::cout << RED << "[Server]Got CGI: " << cgi.first << " -> ";
		std::cout << cgi.second << CRESET << std::endl;
	}
	else
		throw (ParsingException());
}

std::pair<int, short>	ServerConfig::_parse_address(std::string& address)
{
	std::string::size_type	splitter_pos;

	if ((splitter_pos = address.find(':')) != std::string::npos)
		return (std::make_pair(iptoi(address.substr(0, splitter_pos)), std::atoi(address.substr(splitter_pos + 1).c_str())));
	else if (address.find('.') != std::string::npos)
		return (std::make_pair(iptoi(address), 80));
	else 
		return (std::make_pair(0, std::atoi(address.c_str())));
}

void	ServerConfig::_add_location(std::vector<std::string>& tokens)
{
	bool									exact_match = false;
	std::string								location;
	std::vector<ServerLocation>::iterator	pos;

	location = tokens[1];
	if (tokens.size() == 3)
	{
		location = tokens[2];
		if (tokens[1] == "=")
			exact_match = true;
		else
			throw (ParsingException());
	}
	for (pos = locations.begin(); pos != locations.end(); pos++)
	{
		if (exact_match && pos->get_exact_match())
		{
			if (std::count(pos->get_location_match().begin(), pos->get_location_match().end(), '/') <
				std::count(location.begin(), location.end(), '/'))
				break ;
		}
		else if (exact_match && !pos->get_exact_match())
			break ;
		else if (!exact_match && !pos->get_exact_match())
		{
			if (std::count(pos->get_location_match().begin(), pos->get_location_match().end(), '/') <
				std::count(location.begin(), location.end(), '/'))
				break ;
		}
		else
			continue ;
	}
	std::cout << RED << "[Server]Adding location \"" << location << "\"" << CRESET << std::endl;
	pos = locations.insert(pos, ServerLocation(content, location, exact_match));
	pos->fill_default(root, index);
	return ;
}
