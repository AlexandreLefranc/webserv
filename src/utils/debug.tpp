template <class T, class U>
void	display_map(const std::map<T, U>& map, const std::string& name)
{
	for (typename std::map<T, U>::const_iterator it = map.begin(); it != map.end(); it++)
	{
		std::cout << name << "[" << it->first << "] = " << it->second << std::endl;
	}
}

template <class T>
void	display_bits(T obj)
{
	size_t	nbits = sizeof(obj) * 8;
	bool	bit;

	--nbits;
	while (nbits != 0)
	{
		bit = (obj >> nbits) & 1;
		std::cout << bit;
		if (nbits % 8 == 0)
			std::cout << " ";
		--nbits;
	}
	bit = (obj >> nbits) & 1;
	std::cout << bit << std::endl;
	
}