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

const char* ReponseException::what() const throw()
{
	return "ReponseException";
}