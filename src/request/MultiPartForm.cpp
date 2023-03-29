# include "MultiPartForm.hpp"

MultiPartForm::MultiPartForm()
{}

MultiPartForm::MultiPartForm(std::vector<char> form_vec)
{
	std::cout << "Construct of MultiPart" << std::endl;
	std::string form_str = std::string(form_vec.begin(), form_vec.end());
	std::cout << form_str << std::endl;
	
	std::string line = form_str.substr(0, form_str.find("\r\n"));
	while (line != "")
	{
		if (std::count(line.begin(), line.end(), ':') == 0)
		{
			std::cout << YEL << "[Request] 400 Bad Request" << CRESET << std::endl;
			throw RequestParsingException(400);
		}

		std::vector<std::string>	splitted = split_first(line, ":");
		std::string					key = tolowerstr(trim(splitted[0]));
		std::string					value = trim(splitted[1]);
		headers[key] = value;

		if (key == "content-disposition")
		{
			_parse_content_disposition(value);
		}
		form_vec.erase(form_vec.begin(), form_vec.begin() + line.length() + 2);
		form_str.erase(form_str.begin(), form_str.begin() + line.length() + 2);
		line = form_str.substr(0, form_str.find("\r\n"));
	}
	display_map(headers, "MultiPart_header");

	body.insert(body.end(), form_vec.begin() + 2, form_vec.end() - 2);
	// std::cout << "MultiPart body:" << std::endl;
	// std::cout << std::string(body.begin(), body.end()) << std::endl;
	std::cout << "Construct of MultiPart done" << std::endl;
}

MultiPartForm::MultiPartForm(const MultiPartForm& other)
	: headers(other.headers)
	, body(other.body)
{
	return ;
}

MultiPartForm::~MultiPartForm()
{}

MultiPartForm&	MultiPartForm::operator=(const MultiPartForm& other)
{
	if (this != &other)
	{
		headers = other.headers;
		body = other.body;
	}
	return (*this);
}

void	MultiPartForm::_parse_content_disposition(const std::string& value)
{
	std::vector<std::string>	split_content = split(value, ";");
	if (trim(split_content[0]) != "form-data" || split_content.size() < 2)
	{
		std::cout << YEL << "[Request] 400 Bad Request" << CRESET << std::endl;
		throw RequestParsingException(400);
	}

	std::vector<std::string>::const_iterator it;
	for (it = split_content.begin() + 1; it != split_content.end(); ++it)
	{
		std::vector<std::string> split_eq = split(*it, "=");
		if (split_eq.size() != 2)
		{
			std::cout << YEL << "[Request] 400 Bad Request" << CRESET << std::endl;
			throw RequestParsingException(400);
		}

		std::string new_value = trim(split_eq[1]).substr(1, split_eq[1].length() - 2);
		headers[trim(split_eq[0])] = new_value;
	}
}

const std::string&			MultiPartForm::get_header(const std::string& key) const
{
	return (headers.at(key));
}

const std::vector<char>&	MultiPartForm::get_body() const
{
	return body;
}