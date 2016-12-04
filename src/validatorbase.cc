#include <functional>
#include "jsonschemacpp/adaptors.h"
#include "jsonschemacpp/utils.h"
#include "jsonschemacpp/validatorbase.h"

#include <iostream>

using namespace std;

namespace JsonSchema
{

template<typename Adaptor>
ValidatorBase<Adaptor>::ValidatorBase(const std::string& meta_schema_name, const CheckFunctionMap& check_functions) :
	d_meta_schema_name(meta_schema_name), d_check_functions(check_functions)
{
}

template<typename Adaptor>
ValidatorBase<Adaptor>::~ValidatorBase()
{
}

template<typename Adaptor>
SchemaStore<Adaptor>& ValidatorBase<Adaptor>::schemas()
{
	return d_schemas;
}

template<typename Adaptor>
bool ValidatorBase<Adaptor>::validate_schema(typename Adaptor::ConstValue schema)
{
	return validate(schema, d_meta_schema_name);
}

template<typename Adaptor>
bool ValidatorBase<Adaptor>::validate(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue schema)
{
	d_errors.clear();
	d_uri_scopes.clear();
	d_schema_id = "schema";
	d_schema_ref.init(schema);
	d_instance_ref.init(instance);
	d_uri_scopes.emplace_back("", schema);
	return validate_instance(true);
}

template<typename Adaptor>
bool ValidatorBase<Adaptor>::validate(typename Adaptor::ConstValue instance, const string& schema)
{
	d_errors.clear();
	d_uri_scopes.clear();
	d_schema_id = schema;
	d_schema_ref.init(d_schemas.schema(schema));
	d_instance_ref.init(instance);
	d_uri_scopes.emplace_back(schema, d_schemas.schema(schema));
	return validate_instance(true);
}

template<typename Adaptor>
void ValidatorBase<Adaptor>::enter_schema_scope(const PathElement& e)
{
	assert(d_uri_scopes.size() > 0);
	d_schema_ref.enter(e);
	d_uri_scopes.back().level++;
}

template<typename Adaptor>
void ValidatorBase<Adaptor>::enter_schema_scope(const PathElement& e, const string& jsonref)
{
	assert(d_uri_scopes.size() > 0);
	if(jsonref.size() == 0 || jsonref[0] != '#')
	{
		throw NoSchema("Not implemented");
	}
	d_uri_scopes.emplace_back(d_uri_scopes.back().id, d_uri_scopes.back().root);
	typename Adaptor::ConstValue root;
	if(parse_json_pointer(d_uri_scopes.back().root, jsonref.substr(1), d_uri_scopes.back().path, root))
	{
		d_schema_ref.enter(e, root);
	}
	else
	{
		throw NoSchema("No info");
	}
}

template<typename Adaptor>
void ValidatorBase<Adaptor>::exit_schema_scope()
{
	assert(d_uri_scopes.size() > 0);
	d_schema_ref.exit();
	if(d_uri_scopes.back().level > 0)
		d_uri_scopes.back().level--;
	else
		d_uri_scopes.pop_back();
}

template<typename Adaptor>
void ValidatorBase<Adaptor>::enter_instance_scope(const PathElement& e)
{
	d_instance_ref.enter(e);
}

template<typename Adaptor>
void ValidatorBase<Adaptor>::exit_instance_scope()
{
	d_instance_ref.exit();
}

template<typename Adaptor>
void ValidatorBase<Adaptor>::schema_error(const string& msg)
{
	throw SchemaError(schema_path(), msg);
}

template<typename Adaptor>
void ValidatorBase<Adaptor>::add_error(const string& constraint)
{
	d_errors.emplace_back(d_schema_ref.path(), d_instance_ref.path(), constraint);
}

template<typename Adaptor>
bool ValidatorBase<Adaptor>::validate_instance(bool log_errors)
{
	typename Adaptor::ConstValue instance(d_instance_ref.value());
	typename Adaptor::ConstValue schema(d_schema_ref.value());

	assert(schema.is_object());
	bool valid = true;
	for(typename Adaptor::ConstValueIterator it(schema.begin()); it != schema.end(); ++it)
	{
		typename CheckFunctionMap::const_iterator fit(d_check_functions.find(it.name()));
		if(fit != d_check_functions.end())
		{
			if(!fit->second(this, instance, *it, schema, log_errors))
				valid = false;
		}
	}
	return valid;
}

template<typename Adaptor>
unsigned int ValidatorBase<Adaptor>::utf8_length(const string& str)
{
	unsigned int length = 0;
	for(unsigned int k = 0; k < str.size(); k++)
	{
		if((str[k] & 0xC0) != 0x80)
			length++;
	}
	return length;
}

template<typename Adaptor>
bool ValidatorBase<Adaptor>::is_type(typename Adaptor::ConstValue instance, const string& type)
{
	if(type == "array")
		return instance.is_array();
	else if(type == "boolean")
		return instance.is_bool();
	else if(type == "integer")
		return instance.is_integral();
	else if(type == "number")
		return instance.is_numeric();
	else if(type == "null")
		return instance.is_null();
	else if(type == "object")
		return instance.is_object();
	else if(type == "string")
		return instance.is_string();
	return false;
}

}

JSONSCHEMA_INSTANTIATE_TEMPLATE(JsonSchema::ValidatorBase)

