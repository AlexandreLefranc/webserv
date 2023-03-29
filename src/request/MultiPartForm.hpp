#ifndef MULTIPARTFORM_HPP
# define MULTIPARTFORM_HPP

# include <string>
# include <map>
# include <iostream>
# include <algorithm>

# include "webserv.hpp"
# include "utils/Exceptions.hpp"

class MultiPartForm
{
private:
	string_map			headers;
	std::vector<char>	body;

private:
	void	_parse_content_disposition(const std::string& value);

public:
	//	Constructors
	MultiPartForm();
	MultiPartForm(std::vector<char> form);
	MultiPartForm(const MultiPartForm& other);
	//	Destructor
	~MultiPartForm();

	//	Assignment operator.
	MultiPartForm&	operator=(const MultiPartForm& other);

	//	Getters
	const std::string&			get_header(const std::string& key) const;
	const std::vector<char>&	get_body() const;
};

#endif