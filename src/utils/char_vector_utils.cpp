# include <vector>
# include <string>
# include <algorithm>

# include "webserv.hpp"

bool	is_equal(std::vector<char> vec, const std::string& str)
{
	if (vec.size() != str.length())
		return (false);
	return (vec == std::vector<char>(str.begin(), str.end()));
}

std::vector<char>::const_iterator	vec_find(const std::vector<char>& vec, const std::string& substr)
{
	std::vector<char>::const_iterator	it = vec.begin();

	if (substr.length() > vec.size())
		return (vec.end());
	while (it + substr.length() <= vec.end())
	{
		if (is_equal(std::vector<char>(it, it + substr.length()), substr))
			return (it);
		it++;
	}
	return (vec.end());
}

std::vector<char>::const_iterator	vec_find(const std::vector<char>& vec, const std::string& substr, std::vector<char>::const_iterator& start_pos)
{
	std::vector<char>::const_iterator	it = start_pos;

	if (substr.length() > static_cast<unsigned long>(vec.end() - start_pos))
		return (vec.end());
	while (it + substr.length() <= vec.end())
	{
		if (is_equal(std::vector<char>(it, it + substr.length()), substr))
			return (it);
		it++;
	}
	return (vec.end());
}