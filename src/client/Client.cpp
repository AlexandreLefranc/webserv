#include "client/Client.hpp"

Client::Client(int server_fd, const ServerConfig& config)
	: fd(-1)
	, config(config)
	, request()
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
	std::vector<char>	response_vector = response.build_response_vector();

	std::cout << YEL << "[Client] Sending response" << CRESET << std::endl;
	send(fd, response_vector.data(), response_vector.size(), 0);
}