#ifndef deco_floating_point_h
#define deco_floating_point_h

#include "../OutputStream.h"

namespace deco
{
	template<typename Stream, typename T> constexpr
	std::enable_if_t<
		std::is_base_of_v<OutputStream, std::decay_t<Stream>> &&
		std::is_floating_point_v<T>
	>
		write(Stream& stream, const T& value)
	{
		stream.entry(to_string(value));
		//stream.entry(trim_float(std::to_string(value)));
	}

	template<typename T> constexpr
	std::enable_if_t<std::is_floating_point_v<T>>
		read(const Entry& entry, T& value)
	{
		using namespace boost::spirit::x3;
		phrase_parse(entry.content.begin(), entry.content.end(), real_parser<T>(), ascii::space, value);

		//value = stof(std::string(entry.content)); // no string_view/iterators support
	}
}

#endif//guard