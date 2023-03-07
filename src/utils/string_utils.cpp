#include <cctype>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

std::vector<std::string>	split(std::string s, std::string delimiter)
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

std::vector<std::string>	split_first(std::string s, std::string delimiter)
{
	std::vector<std::string>	res;

	size_t pos = s.find(delimiter, 0);
	if (pos == std::string::npos)
	{
		res.push_back(s);
		return res;
	}

	res.push_back(s.substr(0, pos));
	res.push_back(s.substr(pos + delimiter.length(), s.length() - (pos + delimiter.length())));

	return res;
}

std::string		trim(const std::string& str)
{
    size_t first = str.find_first_not_of(" \t\v\r\f");
    if (std::string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(" \t\v\r\f");
    return str.substr(first, (last - first + 1));
}

std::string		tolowerstr(const std::string& str)
{
	std::string	copy(str);
	for (size_t i = 0; i < copy.length(); i++) {
        copy[i] = std::tolower(copy[i]);
    }

	return copy;
}

std::string		toupperstr(const std::string& str)
{
	std::string	copy(str);
	for (size_t i = 0; i < copy.length(); i++) {
        copy[i] = std::toupper(copy[i]);
    }

	return copy;
}