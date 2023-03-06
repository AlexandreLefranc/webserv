#ifndef RESPONSEGENERATOR_HPP
# define RESPONSEGENERATOR_HPP

# include <sys/types.h>
# include <sys/socket.h>

# include <sstream>
# include <string>

# include "request/HTMLGenerator.hpp"

class ResponseGenerator
{
private: // Non instanciable
	ResponseGenerator();

public:
	static void	send_error(int code, const std::string& desc, int client_fd);
};


#endif