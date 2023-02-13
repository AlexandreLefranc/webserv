# include "parse_utils.hpp"

/*==============================================================================
	Exception.
==============================================================================*/

const char*	ParsingException::what() const throw()
{
	return ("HTTP parsing error.");
}

/*==============================================================================
	Parse functions.
==============================================================================*/

std::string&	format_line(std::string& line)
{
	bool					quote = false;
	std::string::iterator	it;

	it = line.find_first_not_of(" \t\v\r\f");
	if (it != std::string::npos)
		line = line.erase(0, it);
	it = line.begin();
	for (; it != line.end(); it++)
	{
		if (*it == '#' && quote == false)
			break;
		if (*it == '\"')
			quote = !quote;
	}
	line.erase(it, line.end());
	it = line.last_not_of(" \t\v\r\f")
	if (it != std::string::npos)
		line.erase(it , line.end());
	return (line);
}

std::vector<std::string>	split_tokens(std::string line)
{
	std::string::iterator		it;
	std::vector<std::string>	tokens;
	std::string					seps = " \t\v\r\f"
	bool						quote = false;

	it = line.begin();
	while (it != line.end())
	{
		if (*it == '\"')
			quote = !quote;
		else if (seps.find(*it) != std::string::npos && !quote)
		{
			tokens.push_back(line.substr(line.begin(), it));
			line = line.erase(line.begin(), it);
			format_line(line);
			it = line.begin();
			continue ;
		}
		it++;
	}
	if (tokens.size() < 2)
		return (std::vector());
	if (tokens[tokens.size() - 1] == ";" || tokens[tokens.size() - 1] == "{" )
		tokens.pop_back();
	else
	{
		it = tokens[tokens.size() - 1].find(';');
		if (it == std::string::npos)
			return (std::vector());
		it = tokens[tokens.size() - 1].erase(it);
		if (it != std::string::npos)
			return (std::vector());
	}
	return (tokens);
}