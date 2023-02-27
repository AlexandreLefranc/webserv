#ifndef CGI_HPP
# define CGI_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include <unistd.h>
# include <sys/wait.h>

# include <vector>
# include <map>
# include <cstdlib>
# include <string>
# include <stdexcept>

# include "webserv.hpp"

# define READ_END	0
# define WRITE_END	1

class CGI
{
private: // Disable defaults behaviors
	CGI();
	CGI(const CGI& src);
	CGI&	operator=(const CGI& src);

private:
	std::string _executable;

public:
	CGI(const std::string& exec);
	~CGI();

	std::string	run_script(const std::string& script) const;
};

#endif