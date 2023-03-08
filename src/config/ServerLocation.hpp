#ifndef SERVERLOCATION_HPP
# define SERVERLOCATION_HPP

# include <sstream>
# include <utility>
# include <set>

# include "parse_utils.hpp"

class ServerLocation
{
public:
	static std::set<std::string>	KnownMethods;

private:
	//	Attributes
	std::stringstream*		content;
	bool					exact_match;
	std::string				location_match;
	std::set<std::string>	methods;
	std::string				root;
	std::string				index;
	bool					autoindex;
	std::string				redirect;

	//	Constructors
	ServerLocation();

public:
	//	Constructor
	ServerLocation(std::stringstream* config, std::string& location_match, bool exact_match);
	ServerLocation(const ServerLocation& other);
	//	Assignment operator
	ServerLocation&	operator=(const ServerLocation& other);
	//	Destructor
	~ServerLocation();

	//	Getters
	bool					get_exact_match() const;
	const std::string&		get_location_match() const;
	std::set<std::string>	get_methods() const;
	const std::string&		get_root() const;
	const std::string&		get_index() const;
	bool					get_autoindex() const;
	const std::string&		get_redirect() const;

	//	Location matching
	bool	location_is_match(const std::string& target) const;
	void	fill_default(std::string serv_root, std::string serv_index);

private:
	void	_parse();
	void	_parse_line(std::string& line);
};

#endif