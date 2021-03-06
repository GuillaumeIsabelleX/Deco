#ifndef deco_unescaped_string_h
#define deco_unescaped_string_h

#include "../InputStream.h"
#include "../OutputStream.h"
#include <string>
#include <strong_type.h>

namespace deco
{
	STRONG_TYPE(unescaped_string, std::string);

	template<typename Stream> constexpr
	std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
	write(Stream& stream, unescaped_string& value) {
		stream.entry(value);
	}

	template<typename I> constexpr
	void read(InputStream<I>& stream, unescaped_string& value)
	{
		value = std::string(stream.parse_entry().content);
	}
}

#endif//guard