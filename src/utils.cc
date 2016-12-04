#include <cassert>
#include <sstream>
#include <utility>
#include "jsonschemacpp/adaptors.h"
#include "jsonschemacpp/utils.h"

using namespace std;

namespace JsonSchema
{

const vector<pair<string,string>> json_pointer_escape_strings { {"~1", "/"}, {"~0", "~"}};

std::string as_string(const Path& path)
{
	stringstream ss;
	for(const PathElement& e : path)
	{
		if(const unsigned int* v = boost::get<unsigned int>(&e))
			ss << "[" << *v << "]";
		else if(const string* v = boost::get<string>(&e))
			ss << "[\"" << *v << "\"]";
		else
			assert(false);
	}
	return ss.str();
}

std::string as_string(const ValidationError& e)
{
	std::stringstream ss;
	ss << "Failed validating instance" << JsonSchema::as_string(e.instance_path()) <<
		" against schema" << JsonSchema::as_string(e.schema_path()) << 
		" for constraint \"" << e.constraint_name() << "\"";
	return ss.str();
}

}

