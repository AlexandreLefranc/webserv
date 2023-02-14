#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP

# include <exception>

class RecvException: public std::exception
{
	virtual const char* what() const throw();
};

class StopException: public std::exception
{
	virtual const char* what() const throw();
};

class CloseClientException: public std::exception
{
	virtual const char* what() const throw();
};

#endif