template <class T, class U>
void	display_map(const std::map<T, U>& map, const std::string& name)
{
	for (typename std::map<T, U>::const_iterator it = map.begin(); it != map.end(); it++)
	{
		std::cout << name << "[" << it->first << "] = " << it->second << std::endl;
	}
}

template <class T>
void	display_vector(const std::vector<T>& vec, const std::string& name)
{
	for (size_t	i = 0; i < vec.size(); ++i)
	{
		std::cout << name << "[" << i << "] = " << vec[i] << std::endl;
	}
}

template <class T>
void	display_bits(const T& obj)
{
	// size_t	nbits = sizeof(obj) * 8;
	// bool	bit;

	// --nbits;
	// while (nbits != 0)
	// {
	// 	bit = (obj >> nbits) & 1;
	// 	std::cout << bit;
	// 	if (nbits % 8 == 0)
	// 		std::cout << " ";
	// 	--nbits;
	// }
	// bit = (obj >> nbits) & 1;
	// std::cout << bit << std::endl;
	
	const char* bytes = reinterpret_cast<const char*>(&obj);

	std::string result;
	result.reserve(sizeof(obj) * 8 + sizeof(obj));


	for (int i = sizeof(obj) - 1; i >= 0; --i)
	{
		for (int j = 7; j >= 0; --j)
		{
			result += (bytes[i] & (1 << j) ? '1' : '0');
		}
		result += ' ';
	}

	std::cout << result << std::endl;
}