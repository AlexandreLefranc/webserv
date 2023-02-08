#include <string>

std::string&	format_line(std::string& line)
{
	std::string				line;
	bool					quote = false;
	std::string::iterator	it;

	line = line.erase(0, line.find_first_not_of(" \t\v\r\f"));
	it = line.begin();
	for (; it != line.end(); it++)
	{
		if (*it == '#' && quote == false)
			break;
		if (*it == '\"')
			quote = !quote;
	}
	line.erase(line.begin(), it);
	line.erase(line.last_not_of(" \t\v\r\f"), line.end());
	return (line);
}