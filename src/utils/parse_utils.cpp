# include "parse_utils.hpp"

/*==============================================================================
	Parse functions.
==============================================================================*/

std::string&	format_line(std::string& line)
{
	bool					quote = false;
	std::string::size_type	pos;
	std::string::iterator	it;

	pos = line.find_first_not_of(" \t\v\r\f");
	if (pos != std::string::npos)
		line = line.erase(0, pos);
	else
		line.clear();
	it = line.begin();
	for (; it != line.end(); it++)
	{
		if (*it == '#' && quote == false)
			break;
		if (*it == '\"')
			quote = !quote;
	}
	line.erase(it, line.end());
	pos = line.find_last_not_of(" \t\v\r\f");
	if (pos != std::string::npos)
		line.erase(pos + 1);
	return (line);
}

std::vector<std::string>	split_tokens(std::string line)
{
	std::string::iterator		it;
	std::string::size_type		pos;
	std::vector<std::string>	tokens;
	std::string					seps = " \t\v\r\f";
	bool						quote = false;

	it = line.begin();
	while (it != line.end())
	{
		if (*it == '\"')
			quote = !quote;
		else if (seps.find(*it) != std::string::npos && !quote)
		{
			// std::cout << "[split]found sep for:\'" << std::string(line.begin(), it) << "\'" << std::endl;
			tokens.push_back(std::string(line.begin(), it));
			// std::cout << "[split]tokens size: " << tokens.size() << std::endl;
			line.erase(line.begin(), it);
			format_line(line);
			// std::cout << "[split]line after split: " << line << std::endl;
			it = line.begin();
			continue ;
		}
		it++;
	}
	tokens.push_back(line);
	// std::cout << "[split]tokens size after split: " << tokens.size() << std::endl;
	if (tokens.size() < 2)
		return (std::vector<std::string>());
	if (tokens[tokens.size() - 1] == ";" || tokens[tokens.size() - 1] == "{" )
		tokens.pop_back();
	else
	{
		pos = tokens.back().find(';');
		if (pos == std::string::npos || tokens.back().size() > pos + 1)
			return (std::vector<std::string>());
		tokens.back().erase(tokens.back().end() - 1);
	}
	return (tokens);
}

int	iptoi(const std::string& ip)
{
	std::stringstream			ss;
	std::vector<int>			seq;
	std::string					segment;
	int							segment_int;
	unsigned int				address = 0;

	ss.str(ip);
	while (std::getline(ss, segment, '.'))
	{
		segment_int = std::atoi(segment.c_str());
		if (segment_int < 0 || segment_int > 255)
			throw std::runtime_error("ip contains number not between 0 and 255");
		seq.push_back(segment_int);
	}
	
	if (seq.size() != 4)
	{
		throw std::runtime_error("ip is not formated x.x.x.x");
	}

	address += (seq[0] << 24);
	address += (seq[1] << 16);
	address += (seq[2] << 8);
	address += (seq[3] << 0);

	return (address);
}