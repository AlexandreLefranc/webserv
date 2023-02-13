#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

std::vector<std::string> split(std::string s, std::string delimiter)
{
	size_t	pos_start = 0;
	size_t	pos_end;
	size_t	delim_len = delimiter.length();

	std::string					token;
	std::vector<std::string>	res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
	{
		token = s.substr (pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back (token);
	}

	res.push_back (s.substr (pos_start));

	res.erase(std::remove(res.begin(), res.end(), ""), res.end());

	return res;
}