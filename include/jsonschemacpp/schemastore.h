#ifndef JSONSCHEMA_SCHEMASTORE_H
#define JSONSCHEMA_SCHEMASTORE_H

#include <map>
#include <string>
#include "jsonschemacpp/exceptions.h"

namespace JsonSchema
{

template<typename Adaptor>
class SchemaStore
{
public:
	SchemaStore();

	void add_schema(const std::string& path, typename Adaptor::ConstValue schema);
	void add_schema(const std::string& path, const std::string& schema);

	typename Adaptor::ConstValue schema(const std::string& path) const;

private:
	std::map<std::string, typename Adaptor::Value> d_schemas;
};

}

#endif

