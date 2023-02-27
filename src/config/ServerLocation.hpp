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
	std::set<t_http_method>		methods;
	std::string					root;
	std::string					index;
	bool						dir_ls;
	std::string					dir_default;
public:
	//	Constructor
	ServerLocation(std::stringstream& config, std::string& location_match, bool exact_match);
	ServerLocation(const ServerLocation& other);
	//	Destructor
	~ServerLocation();
	//	Assignment operator
	ServerLocation&	operator=(const ServerLocation& other);

	//	Getters
	bool								get_exact_match() const;
	const std::string&					get_location_match() const;
	const std::set<http_method_type>&	get_methods() const;
	const std::string&					get_root() const;
	const std::string&					get_index() const;
	bool								get_dir_ls();
	const std::string&					get_dir_default() const;
	std::pair<int, std::string>			get_error_page() const;

	//	Location matching
	bool	locationIsMatch(const std::string& target) const;

private:
	void	_parse();
	void	_parse_line(std::string& line);
};

#endif