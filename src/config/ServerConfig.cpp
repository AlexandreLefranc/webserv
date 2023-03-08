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

// std::string	ServerConfig::get_target(std::string init_target, std::string method) const
// {
// 	const ServerLocation*	matched_location;

// 	try
// 	{
// 		matched_location = get_location(init_target);
// 	}
// 	catch (ResponseException& e)
// 	{
// 		return (init_target);
// 	}
// 	if (matched_location->get_methods().count(method) == 0)
// 		throw (ResponseException());
// 	// if (!matched_location->get_index().empty())
// 	// 	return (get_target(matched_location->get_index(), method));
// 	if (!matched_location->get_root().empty())
// 		return (init_target.replace(0, \
// 		matched_location->get_location_match().length(), matched_location->get_root()));
// 	return (init_target);
// }

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
		std::cout << "Splitted token length: " << tokens.size() << std::endl	;
		_insert_token(tokens);
	}
	return ;
}

void	ServerConfig::_insert_token(std::vector<std::string> tokens)
{
	if (tokens.front() == "root" && tokens.size() == 2 && root.empty())
		root = tokens[1];
	else if (tokens.front() == "server_name" && tokens.size() == 2 && server_name.empty())
		server_name = tokens[1];
	else if (tokens.front() == "listen" && tokens.size() == 2 && listen_port == std::make_pair<int, short>(0, 80))
		listen_port = _parse_address(tokens[1]);
	else if (tokens.front() == "error_page" && tokens.size() == 3)
		error_page.insert(std::make_pair(std::atoi(tokens[1].c_str()), tokens[2]));
	else if (tokens.front() == "index" && tokens.size() == 2 && index == "index.html")
		index = tokens[1];
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

	std::cout << "[_add_location]tokens size: " << tokens.size() << std::endl;
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
	locations.insert(pos, ServerLocation(content, location, exact_match));
	locations.back().fill_default(root, index);
	return ;
}
