#include <string>


typedef struct Status
{
	std::string	protocol("HTTP/1.1");
	int			code;
	std::string	message;
}	t_status;

class Response
{
private:
	t_status										_status;
	std::map<std::string key, std::string value>	_headers;
	std::string										_body;
	
public:
	Response();
	// ~Response();
	Response(const Response& other);
	Response&	operator=(const Response& other);

	void	create(const Request& request, const ServerConfig& config);
}