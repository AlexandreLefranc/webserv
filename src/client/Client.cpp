#include "client/Client.hpp"

Client::Client(int server_fd, const ServerConfig& config)
	: fd(-1)
	, config(config)
	, request(config)
	, request_complete(false)
	, response(request, config)
{
	std::cout << YEL << "[Client] Accept client" << CRESET << std::endl;
	std::cout << YEL << "[Client] OPENING fd: " << CRESET;

	fd = accept(server_fd, NULL, NULL);
	if (fd < 0)
	{
		throw std::runtime_error("accept() failed");
	}
	request.set_client_fd(fd);

	std::cout << YEL << fd << CRESET << std::endl;
}

Client::~Client()
{
	if (fd != -1)
	{
		std::cout << YEL << "[Client] CLOSING fd: " << fd << CRESET << std::endl;
		close(fd);
		fd = -1;
	}
}


void	Client::parse_request()
{
	std::vector<char>	data = receive_all(fd); // can throw
	request_complete = request.parse_data(data); // can throw
	if (request_complete == true)
	{
		std::cout << YEL << "[Request] Request complete!"<< CRESET << std::endl;
	}
}

void	Client::create_response()
{
	response.create();
	return ;
}

void	Client::send_response()
{
	std::cout << YEL << "[Client] Sending response" << CRESET << std::endl;
	// send_example_page(fd);

	std::ostringstream			ss_response;
	const Status&				response_status = response.get_status();
	const string_map&			response_headers = response.get_headers();
	const std::vector<char>&	response_body = response.get_body();

	ss_response	<< response_status.protocol << " " << response_status.code
				<< " " << response_status.message << "\r\n";

	string_map::const_iterator it;
	for (it = response_headers.begin(); it != response_headers.end(); ++it)
	{
		ss_response << it->first << ": " << it->second << "\r\n";
	}
	ss_response << "\r\n";

	ss_response.write(response_body.data(), response_body.size());

	// std::cout << ss_response.str() << std::endl;
	send(fd, ss_response.str().c_str(), ss_response.str().length(), 0);
}