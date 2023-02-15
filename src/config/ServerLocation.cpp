# include "ServerLocation.hpp"

/*==============================================================================

							PUBLIC MEMBER FUNCTIONS.

==============================================================================*/


/*==============================================================================
	Constructors.
==============================================================================*/

ServerLocation::ServerLocation(std::ifstream& file, std::string& location_match, bool exact_match)
	: file(file)
	, location_match(location_match)
	, exact_match(exact_match)
{
	_parse();
	return ;
}

ServerLocation::ServerLocation(const ServerLocation& other)
	: file(other.file)
	, exact_match(other.exact_match)
	, location_match(other.location_match)
	, root(other.root)
	, error_page(other.error_page)
	, index(other.index)
	, try_files(other.try_files)
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
		file = other.file;
		exact_match = other.exact_match;
		location_match = other.location_match;
		root = other.root;
		error_page = other.error_page;
		index = other.index;
		try_files = other.try_files;
	}
	return ;
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

	if (!file.is_open())
		throw (std::ios_base::failure("Config file error."));while (std::getline(file, line))
	while (std::getline(file, line))
	{
		line = format_line(line);
		if (line.empty())
			continue ;
		else if (line == "}")
			return ;
		_parse_line(line);
	}
}

void	ServerLocation::_parse_line()
{
	std::vector	tokens = split_tokens(line);

	if (tokens.front() == "index")
		index.insert(tokens.begin() + 1, tokens.end());
	else if (tokens.front() == "try_files")
		try_files.insert(tokens.begin() + 1, tokens.end());
	else if (tokens.front() == "root" && tokens.size() == 2)
		root = tokens[1];
	else if (tokens.front() == "error_page" && tokens.size() == 3)
		error_page = std::make_pair(tokens[1], tokens[2]);
	else
		throw (ParsingException());
}