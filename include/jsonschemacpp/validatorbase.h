#ifndef JSONSCHEMA_VALIDATORBASE_H
#define JSONSCHEMA_VALIDATORBASE_H

#include <functional>
#include <list>
#include <map>
#include <string>
#include <vector>
#include "jsonschemacpp/schemastore.h"
#include "jsonschemacpp/schematypes.h"

namespace JsonSchema
{

template<class Adaptor>
class ValidatorBase
{
protected:
	typedef std::function<bool(ValidatorBase<Adaptor>*, typename Adaptor::ConstValue,
			typename Adaptor::ConstValue, typename Adaptor::ConstValue, bool)> CheckFunction;
	typedef std::map<std::string, CheckFunction> CheckFunctionMap;

	ValidatorBase(const std::string& meta_schema_name, const CheckFunctionMap& check_functions);

public:
	virtual ~ValidatorBase();

	SchemaStore<Adaptor>& schemas();

	bool validate_schema(typename Adaptor::ConstValue schema);
	bool validate(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue schema);
	bool validate(typename Adaptor::ConstValue instance, const std::string& schema);

	const std::list<ValidationError>& errors() const { return d_errors; }

protected:
	const Path& schema_path() const { return d_schema_ref.path(); }
	const Path& instance_path() const { return d_instance_ref.path(); }

	void enter_schema_scope(const PathElement& e);
	void enter_schema_scope(const PathElement& e, const std::string& jsonref);
	void exit_schema_scope();
	void enter_instance_scope(const PathElement& e);
	void exit_instance_scope();

	void schema_error(const std::string& msg);
	void add_error(const std::string& constraint);
	bool validate_instance(bool log_errors);

	static unsigned int utf8_length(const std::string& str);
	static bool is_type(typename Adaptor::ConstValue instance, const std::string& type);

	template<typename ValidatorClass, bool(ValidatorClass::*f)(typename Adaptor::ConstValue,
			typename Adaptor::ConstValue, typename Adaptor::ConstValue, bool)>
	struct Handler
	{
		static bool invoke(ValidatorBase* v, typename Adaptor::ConstValue instance,
				typename Adaptor::ConstValue constraint, typename Adaptor::ConstValue schema,
				bool log_error)
		{
			return (static_cast<ValidatorClass*>(v)->*f)(instance, constraint, schema, log_error);
		}
	};

	template<typename ValidatorClass, bool(ValidatorClass::*f)(typename Adaptor::ConstValue,
			typename Adaptor::ConstValue, typename Adaptor::ConstValue, bool)>
	static typename CheckFunctionMap::value_type handler(const char* name)
	{
		return typename CheckFunctionMap::value_type(name, &Handler<ValidatorClass, f>::invoke);
	}

	class SchemaScope
	{
	public:
		SchemaScope(ValidatorBase* v, const PathElement& e) : d_v(v) { v->enter_schema_scope(e); }
		SchemaScope(ValidatorBase* v, const PathElement& e, const std::string& jsonref) : d_v(v) {
			v->enter_schema_scope(e, jsonref); }
		~SchemaScope() { d_v->exit_schema_scope(); }
	private:
		ValidatorBase* d_v;
	};

	class InstanceScope
	{
	public:
		InstanceScope(ValidatorBase* v, const PathElement& e) : d_v(v) { v->enter_instance_scope(e); }
		~InstanceScope() { d_v->exit_instance_scope(); }
	private:
		ValidatorBase* d_v;
	};

private:
	struct UriScope
	{
		UriScope(std::string _id, typename Adaptor::ConstValue _root) : id(_id), root(_root), level(0) {}
		std::string id;
		Path path;
		typename Adaptor::ConstValue root;
		unsigned int level;
	};

	const std::string d_meta_schema_name;
	const CheckFunctionMap& d_check_functions;
	SchemaStore<Adaptor> d_schemas;
	std::list<ValidationError> d_errors;
	std::string d_schema_id;
	JsonRef<typename Adaptor::ConstValue> d_schema_ref;
	JsonRef<typename Adaptor::ConstValue> d_instance_ref;
	std::list<UriScope> d_uri_scopes;
};

}

#endif

