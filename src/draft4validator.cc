#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <boost/regex.hpp>
#include "jsonschemacpp/adaptors.h"
#include "jsonschemacpp/draft4validator.h"
#include "jsonschemacpp/metaschema.h"

using namespace std;

namespace JsonSchema
{

template<typename Adaptor>
Draft4Validator<Adaptor>::Draft4Validator() :
	ValidatorBase<Adaptor>(MetaSchema::Draft4::name, draft4_check_functions)
{
}

template<typename Adaptor>
const typename ValidatorBase<Adaptor>::CheckFunctionMap Draft4Validator<Adaptor>::draft4_check_functions
{
	// 5.1 Numeric validators
	ValidatorBase<Adaptor>::template handler<Draft4Validator<Adaptor>, &Draft4Validator<Adaptor>::check_multipleof>("multipleOf"),
	ValidatorBase<Adaptor>::template handler<Draft4Validator<Adaptor>, &Draft4Validator<Adaptor>::check_maximum>("maximum"),
	ValidatorBase<Adaptor>::template handler<Draft4Validator<Adaptor>, &Draft4Validator<Adaptor>::check_minimum>("minimum"),

	// 5.2 String validators
	ValidatorBase<Adaptor>::template handler<Draft4Validator<Adaptor>, &Draft4Validator<Adaptor>::check_maxlength>("maxLength"),
	ValidatorBase<Adaptor>::template handler<Draft4Validator<Adaptor>, &Draft4Validator<Adaptor>::check_minlength>("minLength"),
	ValidatorBase<Adaptor>::template handler<Draft4Validator<Adaptor>, &Draft4Validator<Adaptor>::check_pattern>("pattern"),

	// 5.3 Array validators
	ValidatorBase<Adaptor>::template handler<Draft4Validator<Adaptor>, &Draft4Validator<Adaptor>::check_items>("items"),
	ValidatorBase<Adaptor>::template handler<Draft4Validator<Adaptor>, &Draft4Validator<Adaptor>::check_additionalitems>("additionalItems"),
	ValidatorBase<Adaptor>::template handler<Draft4Validator<Adaptor>, &Draft4Validator<Adaptor>::check_maxitems>("maxItems"),
	ValidatorBase<Adaptor>::template handler<Draft4Validator<Adaptor>, &Draft4Validator<Adaptor>::check_minitems>("minItems"),
	ValidatorBase<Adaptor>::template handler<Draft4Validator<Adaptor>, &Draft4Validator<Adaptor>::check_uniqueitems>("uniqueItems"),

	// 5.4 Object validators
	ValidatorBase<Adaptor>::template handler<Draft4Validator<Adaptor>, &Draft4Validator<Adaptor>::check_maxproperties>("maxProperties"),
	ValidatorBase<Adaptor>::template handler<Draft4Validator<Adaptor>, &Draft4Validator<Adaptor>::check_minproperties>("minProperties"),
	ValidatorBase<Adaptor>::template handler<Draft4Validator<Adaptor>, &Draft4Validator<Adaptor>::check_required>("required"),
	ValidatorBase<Adaptor>::template handler<Draft4Validator<Adaptor>, &Draft4Validator<Adaptor>::check_properties>("properties"),
	ValidatorBase<Adaptor>::template handler<Draft4Validator<Adaptor>, &Draft4Validator<Adaptor>::check_patternproperties>("patternProperties"),
	ValidatorBase<Adaptor>::template handler<Draft4Validator<Adaptor>, &Draft4Validator<Adaptor>::check_additionalproperties>("additionalProperties"),
	ValidatorBase<Adaptor>::template handler<Draft4Validator<Adaptor>, &Draft4Validator<Adaptor>::check_dependencies>("dependencies"),

	// 5.5 Any type validators
	ValidatorBase<Adaptor>::template handler<Draft4Validator<Adaptor>, &Draft4Validator<Adaptor>::check_enum>("enum"),
	ValidatorBase<Adaptor>::template handler<Draft4Validator<Adaptor>, &Draft4Validator<Adaptor>::check_type>("type"),
	ValidatorBase<Adaptor>::template handler<Draft4Validator<Adaptor>, &Draft4Validator<Adaptor>::check_allof>("allOf"),
	ValidatorBase<Adaptor>::template handler<Draft4Validator<Adaptor>, &Draft4Validator<Adaptor>::check_anyof>("anyOf"),
	ValidatorBase<Adaptor>::template handler<Draft4Validator<Adaptor>, &Draft4Validator<Adaptor>::check_oneof>("oneOf"),
	ValidatorBase<Adaptor>::template handler<Draft4Validator<Adaptor>, &Draft4Validator<Adaptor>::check_not>("not")
};

// 5.1 Numeric validators
template<typename Adaptor>
bool Draft4Validator<Adaptor>::check_multipleof(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
		typename Adaptor::ConstValue schema, bool log_errors)
{
	if(!constraint.is_numeric())
		this->schema_error("multipleOf is not numeric");

	double c = constraint.as_double();
	if(c <= 0.0)
		this->schema_error("multipleOf is not positive");

	if(!instance.is_numeric())
		return true;

	double v = instance.as_double();
	double q = v/c;
	bool valid = (int)q == q;

	if(!valid)
	{
		if(log_errors)
			this->add_error("multipleOf");
	}
	return valid;
}

template<typename Adaptor>
bool Draft4Validator<Adaptor>::check_maximum(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
		typename Adaptor::ConstValue schema, bool log_errors)
{
	if(!constraint.is_numeric())
		this->schema_error("maximum is not numeric");

	if(!instance.is_numeric())
		return true;

	double v = instance.as_double();
	double c = constraint.as_double();
	bool valid = false;
	if(schema["exclusiveMaximum"].as_bool())
		valid = v < c;
	else
		valid = v <= c;

	if(!valid)
	{
		if(log_errors)
			this->add_error("maximum");
	}
	return valid;
}

template<typename Adaptor>
bool Draft4Validator<Adaptor>::check_minimum(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
		typename Adaptor::ConstValue schema, bool log_errors)
{
	if(!constraint.is_numeric())
		this->schema_error("minimum is not numeric");

	if(!instance.is_numeric())
		return true;

	double v = instance.as_double();
	double c = constraint.as_double();
	bool valid = false;
	if(schema["exclusiveMinimum"].as_bool())
		valid = v > c;
	else
		valid = v >= c;

	if(!valid)
	{
		if(log_errors)
			this->add_error("minimum");
	}
	return valid;
}

// 5.2 String validators
template<typename Adaptor>
bool Draft4Validator<Adaptor>::check_maxlength(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
		typename Adaptor::ConstValue schema, bool log_errors)
{
	if(!constraint.is_integral())
		this->schema_error("maxLength is not an integer");
	if(constraint.as_int() < 0)
		this->schema_error("maxLength is negative");

	if(!instance.is_string())
		return true;

	unsigned int length = this->utf8_length(instance.as_string());
	bool valid = length <= constraint.as_uint();

	if(!valid)
	{
		if(log_errors)
			this->add_error("maxLength");
	}
	return valid;
}

template<typename Adaptor>
bool Draft4Validator<Adaptor>::check_minlength(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
		typename Adaptor::ConstValue schema, bool log_errors)
{
	if(!constraint.is_integral())
		this->schema_error("minLength is not an integer");
	if(constraint.as_int() < 0)
		this->schema_error("minLength is negative");

	if(!instance.is_string())
		return true;

	unsigned int length = this->utf8_length(instance.as_string());
	bool valid = length >= constraint.as_uint();

	if(!valid)
	{
		if(log_errors)
			this->add_error("minLength");
	}
	return valid;
}

template<typename Adaptor>
bool Draft4Validator<Adaptor>::check_pattern(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
		typename Adaptor::ConstValue schema, bool log_errors)
{
	if(!constraint.is_string())
		this->schema_error("pattern is not a string");

	boost::regex re;
	try
	{
		re.assign(constraint.as_string());
	}
	catch(const boost::regex_error& e)
	{
		this->schema_error("pattern is not a valid regular expression");
	}

	if(!instance.is_string())
		return true;

	bool valid = boost::regex_search(instance.as_string(), re);

	if(!valid)
	{
		if(log_errors)
			this->add_error("pattern");
	}

	return valid;
}

// 5.3 Array validators
template<typename Adaptor>
bool Draft4Validator<Adaptor>::check_items(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
		typename Adaptor::ConstValue schema, bool log_errors)
{
	if(!instance.is_array())
		return true;

	bool valid = true;
	if(constraint.is_object())
	{
		typename ValidatorBase<Adaptor>::SchemaScope schema_scope(this, "items");
		for(unsigned int k = 0; k < instance.size(); k++)
		{
			typename ValidatorBase<Adaptor>::InstanceScope instance_scope(this, k);
			if(!this->validate_instance(log_errors))
				valid = false;
		}
	}
	else if(constraint.is_array())
	{
		typename ValidatorBase<Adaptor>::SchemaScope schema_scope(this, "items");
		for(unsigned int k = 0; k < min(constraint.size(), instance.size()); k++)
		{
			typename ValidatorBase<Adaptor>::SchemaScope schema_item_scope(this, k);
			typename ValidatorBase<Adaptor>::InstanceScope instance_scope(this, k);
			if(!this->validate_instance(log_errors))
				valid = false;
		}
	}
	else
		this->schema_error("items is neither object nor array");

	return valid;
}

template<typename Adaptor>
bool Draft4Validator<Adaptor>::check_additionalitems(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
		typename Adaptor::ConstValue schema, bool log_errors)
{
	if(!instance.is_array())
		return true;

	// If "items" does not exist, it shall be considered an empty object. Children are implicitly valid.
	// If "items" is an object, validation against it is done in check_items. "additionalItems" is irrelevant.
	if(!schema.is_member("items") || schema["items"].is_object())
		return true;

	if(!schema["items"].is_array())
		this->schema_error("items is neither object nor array");

	if(constraint.is_bool())
	{
		// If "additionalItems" is true, any extra children are valid.
		if(constraint.as_bool())
			return true;
		else if(instance.size() > schema["items"].size())
		{
			// If "additionalItems" is false and "items" is an array, the instance can not have more items
			// than in "items".
			if(log_errors)
				this->add_error("additionalItems");
			return false;
		}
		return true;
	}
	else if(constraint.is_object())
	{
		// If "additionalItems" is an object and "items" is an array, the extra items in instance must
		// validate against the "additionalItems" schema.
		bool valid = true;
		typename ValidatorBase<Adaptor>::SchemaScope schema_scope(this, "additionalItems");
		for(unsigned int k = schema["items"].size(); k < instance.size(); k++)
		{
			typename ValidatorBase<Adaptor>::InstanceScope instance_scope(this, k);
			if(!this->validate_instance(log_errors))
				valid = false;
		}
		return valid;
	}
	else
		this->schema_error("additionalItems is neither object nor array");
	return false;
}

template<typename Adaptor>
bool Draft4Validator<Adaptor>::check_maxitems(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
		typename Adaptor::ConstValue schema, bool log_errors)
{
	if(!constraint.is_integral())
		this->schema_error("maxItems is not an integer");
	if(constraint.as_int() < 0)
		this->schema_error("maxItems is negative");
	
	if(!instance.is_array())
		return true;

	bool valid = instance.size() <= constraint.as_uint();
	if(!valid)
	{
		if(log_errors)
			this->add_error("maxItems");
	}
	return valid;
}

template<typename Adaptor>
bool Draft4Validator<Adaptor>::check_minitems(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
		typename Adaptor::ConstValue schema, bool log_errors)
{
	if(!constraint.is_integral())
		this->schema_error("minItems is not an integer");
	if(constraint.as_int() < 0)
		this->schema_error("minItems is negative");

	if(!instance.is_array())
		return true;

	bool valid = instance.size() >= constraint.as_uint();
	if(!valid)
	{
		if(log_errors)
			this->add_error("minItems");
	}
	return valid;
}

template<typename Adaptor>
bool Draft4Validator<Adaptor>::check_uniqueitems(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
		typename Adaptor::ConstValue schema, bool log_errors)
{
	if(!constraint.is_bool())
		this->schema_error("uniqueItems is not a boolean");

	if(!instance.is_array())
		return true;

	if(constraint.as_bool() == false)
		return true;

	for(unsigned int k = 0; k < instance.size(); k++)
	{
		for(unsigned int l = k+1; l < instance.size(); l++)
		{
			if(instance[k] == instance[l])
			{
				if(log_errors)
					this->add_error("uniqueItems");
				return false;
			}
		}
	}

	return true;
}

// 5.4 Object validators
template<typename Adaptor>
bool Draft4Validator<Adaptor>::check_maxproperties(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
		typename Adaptor::ConstValue schema, bool log_errors)
{
	if(!constraint.is_integral())
		this->schema_error("maxProperties is not an integer");
	if(constraint.as_int() < 0)
		this->schema_error("maxProperties is negative");

	if(!instance.is_object())
		return true;

	bool valid = instance.size() <= constraint.as_uint();
	if(!valid)
	{
		if(log_errors)
			this->add_error("maxProperties");
	}
	return valid;
}

template<typename Adaptor>
bool Draft4Validator<Adaptor>::check_minproperties(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
		typename Adaptor::ConstValue schema, bool log_errors)
{
	if(!constraint.is_integral())
		this->schema_error("minProperties is not an integer");
	if(constraint.as_int() < 0)
		this->schema_error("minProperties is negative");

	if(!instance.is_object())
		return true;

	bool valid = instance.size() >= constraint.as_uint();
	if(!valid)
	{
		if(log_errors)
			this->add_error("minProperties");
	}
	return valid;
}

template<typename Adaptor>
bool Draft4Validator<Adaptor>::check_required(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
		typename Adaptor::ConstValue schema, bool log_errors)
{
	if(!constraint.is_array())
		this->schema_error("required is not an array");

	if(!instance.is_object())
		return true;

	bool valid = true;
	for(typename Adaptor::ConstValue property : constraint)
	{
		if(!property.is_string())
			this->schema_error("required has non-string elements");

		string p(property.as_string());
		if(!instance.is_member(p))
		{
			if(log_errors)
				this->add_error("required");
			valid = false;
		}
	}
	return valid;
}

template<typename Adaptor>
bool Draft4Validator<Adaptor>::check_properties(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
		typename Adaptor::ConstValue schema, bool log_errors)
{
	if(!constraint.is_object())
		this->schema_error("properties is not an object");

	if(!instance.is_object())
		return true;

	bool valid = true;
	typename ValidatorBase<Adaptor>::SchemaScope schema_scope(this, "properties");
	for(typename Adaptor::ConstValueIterator it(constraint.begin()); it != constraint.end(); ++it)
	{
		string name(it.name());
		if(instance.is_member(name))
		{
			typename ValidatorBase<Adaptor>::SchemaScope property_scope(this, name);
			typename ValidatorBase<Adaptor>::InstanceScope instance_scope(this, name);
			if(!this->validate_instance(log_errors))
				valid = false;
		}
	}
	return valid;
}

template<typename Adaptor>
bool Draft4Validator<Adaptor>::check_patternproperties(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
		typename Adaptor::ConstValue schema, bool log_errors)
{
	if(!constraint.is_object())
		this->schema_error("patternProperties is not an object");

	if(!instance.is_object())
		return true;

	bool valid = true;
	typename ValidatorBase<Adaptor>::SchemaScope schema_scope(this, "patternProperties");
	for(typename Adaptor::ConstValueIterator cit(constraint.begin()); cit != constraint.end(); ++cit)
	{
		typename ValidatorBase<Adaptor>::SchemaScope property_scope(this, cit.name());
		boost::regex re;
		try
		{
			re.assign(cit.name());
		}
		catch(const boost::regex_error& e)
		{
			this->schema_error("pattern is not a valid regular expression");
		}

		for(typename Adaptor::ConstValueIterator it(instance.begin()); it != instance.end(); ++it)
		{
			string name(it.name());
			if(boost::regex_search(name, re))
			{
				typename ValidatorBase<Adaptor>::InstanceScope instance_scope(this, name);
				if(!this->validate_instance(log_errors))
					valid = false;
			}
		}
	}
	return valid;
}

template<typename Adaptor>
bool Draft4Validator<Adaptor>::check_additionalproperties(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
		typename Adaptor::ConstValue schema, bool log_errors)
{
	if(!instance.is_object())
		return true;

	if(constraint.is_bool() && constraint.as_bool())
		return true;

	typename Adaptor::ConstValue properties;
	if(schema.is_member("properties"))
		properties = schema["properties"];

	vector<string> extra_properties;
	for(typename Adaptor::ConstValueIterator it(instance.begin()); it != instance.end(); ++it)
		extra_properties.push_back(it.name());

	if(properties)
	{
		vector<string> tmp;
		for(const string& p : extra_properties)
		{
			if(!properties.is_member(p))
				tmp.push_back(p);
		}
		tmp.swap(extra_properties);
	}

	if(schema.is_member("patternProperties"))
	{
		typename Adaptor::ConstValue pattern = schema["patternProperties"];
		for(typename Adaptor::ConstValueIterator it(pattern.begin()); it != pattern.end(); ++it)
		{
			boost::regex pattern_re;
			try
			{
				pattern_re.assign(it.name());
			}
			catch(const boost::regex_error& e)
			{
				this->schema_error("patternProperties contains invalid regular expression");
			}

			vector<string> tmp;
			for(const string& p : extra_properties)
			{
				if(!boost::regex_search(p, pattern_re))
					tmp.push_back(p);
			}
			tmp.swap(extra_properties);
		}
	}

	bool valid = true;
	if(constraint.is_bool())
	{
		if(constraint.as_bool() == false && extra_properties.size() > 0)
		{
			if(log_errors)
				this->add_error("additionalProperties");
			valid = false;
		}
	}
	else if(constraint.is_object())
	{
		typename ValidatorBase<Adaptor>::SchemaScope schema_scope(this, "additionalProperties");
		for(const string& p : extra_properties)
		{
			typename ValidatorBase<Adaptor>::InstanceScope instance_scope(this, p);
			if(!this->validate_instance(log_errors))
			{
				valid = false;
			}
		}
	}
	else
		this->schema_error("additionalProperties is neither object nor array");

	return valid;
}

template<typename Adaptor>
bool Draft4Validator<Adaptor>::check_dependencies(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
		typename Adaptor::ConstValue schema, bool log_errors)
{
	if(!constraint.is_object())
		this->schema_error("dependencies is not an object");
	if(!instance.is_object())
		return true;

	bool valid = true;
	for(typename Adaptor::ConstValueIterator it(constraint.begin()); it != constraint.end(); ++it)
	{
		string name(it.name());
		if(instance.is_member(name))
		{
			if((*it).is_object())
			{
				typename ValidatorBase<Adaptor>::SchemaScope schema_scope_0(this, "dependencies");
				typename ValidatorBase<Adaptor>::SchemaScope schema_scope_1(this, name);
				if(!this->validate_instance(log_errors))
				{
					valid = false;
				}
			}
			else if((*it).is_array())
			{
				for(typename Adaptor::ConstValue dep : *it)
				{
					if(!instance.is_member(dep.as_string()))
					{
						if(log_errors)
							this->add_error("dependencies");
						valid = false;
					}
				}
			}
			else
				this->schema_error("dependencies has invalid property");
		}
	}
	return valid;
}

// 5.5 Any type validators
template<typename Adaptor>
bool Draft4Validator<Adaptor>::check_enum(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
		typename Adaptor::ConstValue schema, bool log_errors)
{
	if(!constraint.is_array())
		this->schema_error("enum is not an array");

	for(typename Adaptor::ConstValue c : constraint)
	{
		if(instance == c)
			return true;
	}
	if(log_errors)
		this->add_error("enum");
	return false;
}

template<typename Adaptor>
bool Draft4Validator<Adaptor>::check_type(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
		typename Adaptor::ConstValue schema, bool log_errors)
{
	if(constraint.is_string())
	{
		if(this->is_type(instance, constraint.as_string()))
			return true;
		else
		{
			if(log_errors)
				this->add_error("type");
			return false;
		}
	}
	else if(constraint.is_array())
	{
		for(typename Adaptor::ConstValueIterator it(constraint.begin()); it != constraint.end(); ++it)
		{
			if(this->is_type(instance, (*it).as_string()))
				return true;
		}
		if(log_errors)
			this->add_error("type");
		return false;
	}
	else
		this->schema_error("type is neither object nor array");
	return false;
}

template<typename Adaptor>
bool Draft4Validator<Adaptor>::check_allof(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
		typename Adaptor::ConstValue schema, bool log_errors)
{
	if(!constraint.is_array())
		this->schema_error("allOf is not an array");

	unsigned int num_valid = 0;
	{
		typename ValidatorBase<Adaptor>::SchemaScope schema_scope(this, "allOf");
		for(unsigned int k = 0; k < constraint.size(); k++)
		{
			typename ValidatorBase<Adaptor>::SchemaScope sub_scope(this, k);
			if(this->validate_instance(false))
				num_valid++;
		}
	}

	if(num_valid == constraint.size())
		return true;

	if(log_errors)
		this->add_error("allOf");
	return false;
}

template<typename Adaptor>
bool Draft4Validator<Adaptor>::check_anyof(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
		typename Adaptor::ConstValue schema, bool log_errors)
{
	if(!constraint.is_array())
		this->schema_error("anyOf is not an array");

	unsigned int num_valid = 0;
	{
		typename ValidatorBase<Adaptor>::SchemaScope schema_scope(this, "anyOf");
		for(unsigned int k = 0; k < constraint.size(); k++)
		{
			typename ValidatorBase<Adaptor>::SchemaScope sub_scope(this, k);
			if(this->validate_instance(false))
				num_valid++;
		}
	}

	if(num_valid >= 1)
		return true;

	if(log_errors)
		this->add_error("anyOf");
	return false;
}

template<typename Adaptor>
bool Draft4Validator<Adaptor>::check_oneof(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
		typename Adaptor::ConstValue schema, bool log_errors)
{
	if(!constraint.is_array())
		this->schema_error("oneOf is not an array");

	unsigned int num_valid = 0;
	{
		typename ValidatorBase<Adaptor>::SchemaScope schema_scope(this, "oneOf");
		for(unsigned int k = 0; k < constraint.size(); k++)
		{
			typename ValidatorBase<Adaptor>::SchemaScope sub_scope(this, k);
			if(this->validate_instance(false))
				num_valid++;
		}
	}

	if(num_valid == 1)
		return true;

	if(log_errors)
		this->add_error("oneOf");
	return false;
}

template<typename Adaptor>
bool Draft4Validator<Adaptor>::check_not(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
		typename Adaptor::ConstValue schema, bool log_errors)
{
	if(!constraint.is_object())
		this->schema_error("not is not an object");

	{
		typename ValidatorBase<Adaptor>::SchemaScope schema_scope(this, "not");
		if(this->validate_instance(false))
		{
			if(log_errors)
				this->add_error("not");
			return false;
		}
	}
	return true;
}

}

JSONSCHEMA_INSTANTIATE_TEMPLATE(JsonSchema::Draft4Validator)

