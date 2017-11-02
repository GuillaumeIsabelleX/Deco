#ifndef deco_OutputStream_h
#define deco_OutputStream_h

#include "Traits.h"
#include "deco.h"
#include <gs/Core.h>
#include <string>
#include <vector>

namespace deco
{
	struct OutputStream
	{
		std::string str;

		void entry(const std::string& content)
		{
			indent();
			(str += content) += '\n';
		}

		void begin_set(std::string&& content)
		{
			entry(content += ':');
			++indent_level;
		}
		void begin_set(const std::string_view& content)
		{
			begin_set(std::string(content));
		}

		void end_set()
		{
			--indent_level;
			entry(std::string(":"));
		}

	protected:
		unsigned indent_level = 0;

		void indent()
		{
			for (unsigned n = 0; n < indent_level; ++n)
				str += '\t';
		}
	};

	// utility function for trimming trailing zeros and decimal point after using std::to_string
	auto& trim_float(std::string& str) {
		const auto pos = str.find_last_not_of('0');
		str.erase(pos + (str[pos] != '.'));	// if last character isn't a decimal point, don't delete it
		return str;
	}

	// escape content delimiters
	auto escape_content(const std::string_view content)
	{
		std::string str;

		if (content.empty())
			return str;

		// escape content starting with whitespace or content delimiter
		const auto first = content.front();
		if (first == ' ' ||
			first == '\t' ||
			first == deco::content_delimiter)
			str += deco::content_delimiter;

		str += content;

		// escape content ending with content delimiter or structure delimiter
		const auto last = content.back();
		if (last == deco::structure_delimiter ||
			last == deco::content_delimiter)
			str += deco::content_delimiter;

		return str;
	}

	// remove delimiters used to escape content
	auto unescape_content(std::string_view& content)
	{
		// erase start content delimiter
		if (content.front() == content_delimiter)
			content.remove_prefix(1);

		// erase end content delimiter
		if (content.back() == content_delimiter)
			content.remove_suffix(1);
	}
}


namespace gs
{
	template<>
	struct is_output<deco::OutputStream> : std::true_type {};

	template<>
	struct is_deco<deco::OutputStream> : std::true_type {};


	// serialize output deco
	template<typename Stream, typename T>
	typename std::enable_if_t<
		is_deco_v<Stream> &&
		is_output_v<Stream>
	>
	serialize(Stream& stream, T& value)
	{
		write(stream, value);
	}

	//automatically provide default serialization implementation for arithmetic types
	template<typename T>
	typename std::enable_if_t<std::is_integral_v<T>>
	write(deco::OutputStream& stream, const T& value) {
		stream.entry(std::to_string(value));
	}

	template<typename T>
	typename std::enable_if_t<std::is_floating_point_v<T>>
	write(deco::OutputStream& stream, const T& value) {
		stream.entry(deco::trim_float(std::to_string(value)));
	}

	void write(deco::OutputStream& stream, const std::string& value) {
		stream.entry(value);
	}

	template<typename T>
	void write(deco::OutputStream& stream, std::vector<T>& value)
	{
		for (auto& e : value)
			serialize(stream, e);
	}
}

#endif//guard