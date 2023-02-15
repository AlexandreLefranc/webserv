#ifndef SERVERLOCATION_HPP
# define SERVERLOCATION_HPP

# include <sstream>
# include <utility>

# include "parse_utils.hpp"

class ServerLocation
{
private:
	//	Attributes
	std::stringstream&			content;
	bool						exact_match;
	std::string					location_match;
	std::string					root;
	std::vector<std::string>	index;
	std::vector<std::string>	try_files;
	std::pair<int, std::string>	error_page;

public:
	//	Constructor
	ServerLocation(std::stringstream& config, std::string& location_match, bool exact_match);
	ServerLocation(const ServerLocation& other);
	//	Destructor
	~ServerLocation();
	//	Assignment operator
	ServerLocation&	operator=(const ServerLocation& other);

	//	Getters
	bool						get_exact_match() const;
	std::string					get_location_match() const;
	std::string					get_root() const;
	std::vector<std::string>	get_index() const;
	std::vector<std::string>	get_try_files() const;
	std::pair<int, std::string>	get_error_page() const;

private:
	void	_parse();
	void	_parse_line(std::string& line);
};

#endif