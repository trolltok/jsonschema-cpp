#ifndef JSONSCHEMA_UTILS_H
#define JSONSCHEMA_UTILS_H

#include <sstream>
#include <string>
#include <vector>
#include "jsonschemacpp/schematypes.h"

namespace JsonSchema
{

template<typename ConstValue>
std::string value_as_string(ConstValue v, int depth = -1);
std::string as_string(const Path& path);
std::string as_string(const ValidationError& e);
template<typename ConstValue>
bool parse_json_pointer(ConstValue root, const std::string& json_pointer, Path& path, ConstValue& value);

/* Definitions */

extern const std::vector<std::pair<std::string,std::string>> json_pointer_escape_strings;

template<typename ConstValue>
std::string value_as_string(ConstValue v, int depth)
{
	std::stringstream ss;
	if(v.is_null())
		ss << "null";
	else if(v.is_integral())
		ss << v.as_int();
	else if(v.is_numeric())
		ss << v.as_double();
	else if(v.is_string())
		ss << "\"" << v.as_string() << "\"";
	else if(v.is_bool())
		ss << (v.as_bool() ? "true" : "false");
	else if(v.is_array())
	{
		ss << "[";
		if(depth == 0)
			ss << "...";
		else
		{
			bool first = true;
			for(const auto& t : v)
			{
				if(first)
					first = false;
				else
					ss << ", ";
				ss << value_as_string(t, depth-1);
			}
		}
		ss << "]";
	}
	else if(v.is_object())
	{
		ss << "{";
		if(depth == 0)
			ss << "...";
		else
		{
			bool first = true;
			for(typename ConstValue::const_iterator it(v.begin()); it != v.end(); ++it)
			{
				if(first)
					first = false;
				else
					ss << ", ";
				ss << it.name() << ": " << value_as_string(*it, depth-1);
			}
		}
		ss << "}";
	}
	return ss.str();
}

inline void replace_all(std::string& str, const std::string& pattern, const std::string& replace)
{
	size_t k = 0;
	while((k = str.find(pattern, k)) != std::string::npos)
	{
		str.replace(k, pattern.size(), replace);
		k += replace.size();
	}
}

template<typename ConstValue>
bool parse_json_pointer(ConstValue root, const std::string& json_pointer, Path& path, ConstValue& value)
{
	ConstValue v(root);
	std::string ptr(json_pointer);
	std::string token;

	while(ptr.size() > 0)
	{
		if(ptr[0] != '/')
			return false;
		size_t n = ptr.find('/', 1);
		if(n == std::string::npos)
		{
			token = ptr.substr(1);
			ptr.clear();
		}
		else
		{
			token = ptr.substr(1, n-1);
			ptr = ptr.substr(n);
		}

		for(const std::pair<std::string,std::string>& t : json_pointer_escape_strings)
			replace_all(token, t.first, t.second);

		if(v.is_object())
		{
			if(!v.is_member(token))
				return false;
			v = v[token];
			path.push_back(token);
		}
		else if(v.is_array())
		{
			size_t k;
			unsigned int index = stoul(token, &k);
			if(k != token.size())
				return false;
			if(!v.is_valid_index(index))
				return false;
			v = v[index];
			path.push_back(index);
		}
		else
		{
			return false;
		}
	}

	value = v;
	return true;
}

}

#endif

