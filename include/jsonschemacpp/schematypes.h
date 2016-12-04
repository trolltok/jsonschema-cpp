#ifndef JSONSCHEMA_SCHEMATYPES_H
#define JSONSCHEMA_SCHEMATYPES_H

#include <cassert>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <boost/variant.hpp>

namespace JsonSchema
{

typedef boost::variant<unsigned int, std::string> PathElement;
typedef std::vector<PathElement> Path;

template<typename ConstValue>
class JsonRef
{
public:
	JsonRef() {}
	JsonRef(ConstValue root) { init(root); }

	const Path& path() const { return d_path; }

	void init(ConstValue root)
	{
		d_path.clear();
		d_refs.clear();
		d_refs.push_back(root);
	}

	ConstValue value() const
	{
		assert(d_refs.size() > 0);
		return d_refs.back();
	}

	void enter(const PathElement& e)
	{
		assert(d_refs.size() > 0);
		ConstValue v(d_refs.back());
		if(const unsigned int* index = boost::get<unsigned int>(&e))
		{
			assert(v.is_array());
			assert(v.is_valid_index(*index));
			v = v[*index];
		}
		else if(const std::string* member = boost::get<std::string>(&e))
		{
			assert(v.is_object());
			assert(v.is_member(*member));
			v = v[*member];
		}
		else
			assert(false);

		d_path.push_back(e);
		d_refs.push_back(v);
	}

	void enter(const PathElement& e, ConstValue v)
	{
		assert(d_refs.size() > 0);
		d_path.push_back(e);
		d_refs.push_back(v);
	}

	void exit()
	{
		assert(d_path.size() > 0);
		assert(d_refs.size() > 0);
		d_path.pop_back();
		d_refs.pop_back();
	}

private:
	Path d_path;
	std::vector<ConstValue> d_refs;
};

class ValidationError
{
public:
	ValidationError(const Path& schema_path, const Path& instance_path, const std::string& constraint_name) :
		d_schema_path(schema_path),
		d_instance_path(instance_path),
		d_constraint_name(constraint_name)
	{
	}

	const Path& schema_path() const { return d_schema_path; }
	const Path& instance_path() const { return d_instance_path; }
	const std::string& constraint_name() const { return d_constraint_name; }

private:
	Path d_schema_path;
	Path d_instance_path;
	std::string d_constraint_name;
};

}

#endif

