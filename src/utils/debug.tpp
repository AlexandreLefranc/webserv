template <class T, class U>
void	display_map(const std::map<T, U>& map, const std::string& name)
{
	for (typename std::map<T, U>::const_iterator it = map.begin(); it != map.end(); it++)
	{
		std::cout << name << "[" << it->first << "] = " << it->second << std::endl;
	}
}