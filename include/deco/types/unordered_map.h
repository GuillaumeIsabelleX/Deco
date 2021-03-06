#ifndef deco_std_unordered_map_h
#define deco_std_unordered_map_h

#include "../list_container.h"
#include <unordered_map>

namespace deco
{
	template<typename... Args>
	struct is_list_container<std::unordered_map<Args...>> : std::true_type {};

	template<typename Stream, typename... Args> constexpr
	void write_element_lists(Stream& stream, std::unordered_map<Args...>& value)
	{
		write_key_value_lists(stream, value);
	}

	template<typename Stream, typename... Args> constexpr
	void read_element_lists(Stream& stream, std::unordered_map<Args...>& value)
	{
		read_key_value_lists(stream, value);
	}
}

#endif//guard
