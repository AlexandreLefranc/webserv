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
{
	_parse();
	return ;
}

ServerLocation::ServerLocation(const ServerLocation& other)
	: content(other.content)
	, exact_match(other.exact_match)
	, location_match(other.location_match)
	, root(other.root)
	, index(other.index)
	, try_files(other.try_files)
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
		root = other.root;
		error_page = other.error_page;
		index = other.index;
		try_files = other.try_files;
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

std::string					ServerLocation::get_root() const
{
	return (root);
}

std::vector<std::string>	ServerLocation::get_index() const
{
	return (index);
}

std::vector<std::string>	ServerLocation::get_try_files() const
{
	return (try_files);
}

std::pair<int, std::string>	ServerLocation::get_error_page() const
{
	return (error_page);
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
	if (tokens.front() == "index" && index.empty())
		index.assign(tokens.begin() + 1, tokens.end());
	else if (tokens.front() == "try_files" && try_files.empty())
		try_files.assign(tokens.begin() + 1, tokens.end());
	else if (tokens.front() == "root" && tokens.size() == 2 && root.empty())
		root = tokens[1];
	else if (tokens.front() == "error_page" && tokens.size() == 3 && error_page.second.empty())
		error_page = std::make_pair(std::atoi(tokens[1].c_str()), tokens[2]);
	else
		throw (ParsingException());
}