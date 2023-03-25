#include "utils/Exceptions.hpp"

const char* RecvException::what() const throw()
{
	return "RecvException";
}

const char* StopException::what() const throw()
{
	return "StopException";
}

const char* CloseClientException::what() const throw()
{
	return "CloseClientException";
}

const char* ParsingException::what() const throw()
{
	return "ParsingException";
}

const char* ResponseException::what() const throw()
{
	return "ResponseException";
}

RequestParsingException::RequestParsingException(int code)
	: code(code)
{}

const char*	RequestParsingException::what() const throw()
{
	return "RequestParsingException";
}