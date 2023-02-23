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