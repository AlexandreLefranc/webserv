# include "ServerLocation.hpp"

/*==============================================================================

							PUBLIC MEMBER FUNCTIONS.

==============================================================================*/


/*==============================================================================
	Constructors.
==============================================================================*/

ServerLocation::ServerLocation(std::stringstream& config, std::string& location_match, bool exact_match)
	: content(config)
	, exact_match(exact_match)
	, location_match(location_match)
	, dir_ls(false)
{
	_parse();
	return ;
}

ServerLocation::ServerLocation(const ServerLocation& other)
	: content(other.content)
	, exact_match(other.exact_match)
	, location_match(other.location_match)
	, methods(other.methods)
	, root(other.root)
	, index(other.index)
	, dir_ls(other.dir_ls)
	, dir_default(other.dir_default)
	, error_page(other.error_page)
{
	return ;
}

/*==============================================================================
	Destructors.
==============================================================================*/

ServerLocation::~ServerLocation()
{
	return ;
}

/*==============================================================================
	Assignment operator.
==============================================================================*/

ServerLocation&	ServerLocation::operator=(const ServerLocation& other)
{
	if (&other == this)
	{
		content.str(other.content.str());
		exact_match = other.exact_match;
		location_match = other.location_match;
		methods = other.methods;
		root = other.root;
		error_page = other.error_page;
		index = other.index;
		dir_ls = other.dir_ls;
		dir_default = other.dir_default;
		error_page = other.error_page;
	}
	return (*this);
}

/*==============================================================================
	Getters.
==============================================================================*/

bool						ServerLocation::get_exact_match() const
{
	return (exact_match);
}

std::string					ServerLocation::get_location_match() const
{
	return (location_match);
}

std::set<http_method_type>	ServerLocation::get_methods() const;
{
	return (methods);
}

std::string					ServerLocation::get_root() const
{
	return (root);
}

const std::string&			ServerLocation::get_index() const
{
	return (index);
}

bool						ServerLocation::get_dir_ls();
{
	return (dir_ls);
}

const std::string&			ServerLocation::get_dir_default() const
{
	return (dir_default);
}

std::pair<int, std::string>	ServerLocation::get_error_page() const
{
	return (error_page);
}

/*==============================================================================
	Location matching.
==============================================================================*/

bool	location_is_match(const std::string& target) const
{
	if (exact_match && location_match == target)
		return (true);
	if (location_match == target.substr(0, location_match.length()))
		return (true);
	return (false);
}

/*==============================================================================

							PRIVATE MEMBER FUNCTIONS.

==============================================================================*/

/*==============================================================================
	Parsing functions.
==============================================================================*/

void	ServerLocation::_parse()
{
	std::string	line;

	while (std::getline(content, line))
	{
		std::cout << "[ServerLocation]line: " << line << std::endl;
		line = format_line(line);
		if (line.empty())
			continue ;
		else if (line == "}")
			return ;
		_parse_line(line);
	}
}

void	ServerLocation::_parse_line(std::string& line)
{
	std::vector<std::string>	tokens = split_tokens(line);

	if (tokens.size() == 0)
		throw (ParsingException());
	if (tokens.front() == "index" && tokens.size() == 2 && index.empty())
		index = tokens[1];
	else if (tokens.front() == "methods" && methods.empty())
	{
		for (std::vector<st::string>::iterator	it = tokens.begin() + 1; it != tokens.end(); it++)
		{
			if (*it == "GET")
				methods.insert(GET);
			else if (*it == "DELETE")
				methods.insert(DELETE);
			else if (*it == "POST")
				methods.insert(POST);
			else
				throw (ParsingException());
		}
	}
	else if (tokens.front() == "dir_ls" && tokens.size() <= 2)
	{
		if (tokens.size() == 1 || tokens[1] == "true" || tokens[1] == "1")
			dir_ls = true;
		else if (tokens[1] == "false" || tokens[1] == "0")
			dir_ls = false;
		else
			throw (ParsingException());
	}
	else if (tokens.front() == "dir_default" && tokens.size() == 2)
		dir_default = tokens[1];
	else if (tokens.front() == "root" && tokens.size() == 2 && root.empty())
		root = tokens[1];
	else if (tokens.front() == "error_page" && tokens.size() == 3 && error_page.second.empty())
		error_page = std::make_pair(std::atoi(tokens[1].c_str()), tokens[2]);
	else
		throw (ParsingException());
}