# include "ServerLocation.hpp"

/*==============================================================================

							PUBLIC MEMBER FUNCTIONS.

==============================================================================*/

std::set<std::string>	ServerLocation::KnownMethods;

/*==============================================================================
	Constructors.
==============================================================================*/

ServerLocation::ServerLocation(std::stringstream* config, std::string& location_match, bool exact_match)
	: content(config)
	, exact_match(exact_match)
	, location_match(location_match)
	, autoindex(false)
{
	KnownMethods.insert("GET");
	KnownMethods.insert("POST");
	KnownMethods.insert("DELETE");
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
	, autoindex(other.autoindex)
	, redirect(other.redirect)
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
		content->str(other.content->str());
		exact_match = other.exact_match;
		location_match = other.location_match;
		methods = other.methods;
		root = other.root;
		index = other.index;
		autoindex = other.autoindex;
		redirect = other.redirect;
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

const std::string&			ServerLocation::get_location_match() const
{
	return (location_match);
}

std::set<std::string>		ServerLocation::get_methods() const
{
	return (methods);
}

const std::string&			ServerLocation::get_root() const
{
	return (root);
}

const std::string&			ServerLocation::get_index() const
{
	return (index);
}

bool						ServerLocation::get_autoindex() const
{
	return (autoindex);
}

const std::string&			ServerLocation::get_redirect() const
{
	return (redirect);
}

/*==============================================================================
	Location matching.
==============================================================================*/

bool	ServerLocation::location_is_match(const std::string& target) const
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

	while (std::getline(*content, line))
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
	else if (tokens.front() == "root" && tokens.size() == 2 && root.empty())
		root = tokens[1];
	else if (tokens.front() == "methods" && methods.empty())
	{
		for (std::vector<std::string>::iterator	it = tokens.begin() + 1; it != tokens.end(); it++)
		{
			if (KnownMethods.count(*it) == 0)
				throw (ParsingException());
			methods.insert(*it);
		}
	}
	else if (tokens.front() == "autoindex" && tokens.size() <= 2)
	{
		if (tokens.size() == 1 || tokens[1] == "on" || tokens[1] == "1")
			autoindex = true;
		else if (tokens[1] == "off" || tokens[1] == "0")
			autoindex = false;
		else
			throw (ParsingException());
	}
	else if (tokens.front() == "redirect" && redirect.empty())
		redirect = tokens[1];
	else
		throw (ParsingException());
}