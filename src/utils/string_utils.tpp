template <typename T>
std::string	nbtostr(T nb)
{
	std::ostringstream ss;
	ss << nb;
	return ss.str();
}