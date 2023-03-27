#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP

# include <exception>
# include <string>

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

class ParsingException: public std::exception
{
public:
	virtual const char*	what() const throw();
};

class ResponseException: public std::exception
{
public:
	virtual const char*	what() const throw();
};

class RequestParsingException: public std::exception
{
public:
	int code;
	RequestParsingException(int code);
	virtual const char*	what() const throw();
};

#endif