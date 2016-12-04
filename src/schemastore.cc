#include "jsonschemacpp/adaptors.h"
#include "jsonschemacpp/metaschema.h"
#include "jsonschemacpp/schemastore.h"

using namespace std;

namespace JsonSchema
{

template<typename Adaptor>
SchemaStore<Adaptor>::SchemaStore()
{
	add_schema(MetaSchema::Draft4::name, MetaSchema::Draft4::schema);
}

template<typename Adaptor>
void SchemaStore<Adaptor>::add_schema(const string& path, typename Adaptor::ConstValue schema)
{
	d_schemas.emplace(path, typename Adaptor::Value(schema));
}

template<typename Adaptor>
void SchemaStore<Adaptor>::add_schema(const string& path, const string& schema)
{
	auto t = d_schemas.emplace(path, typename Adaptor::Value());
	if(!t.first->second.parse(schema))
	{
		throw ParseError(string("Error parsing schema " + schema));
	}
}

template<typename Adaptor>
typename Adaptor::ConstValue SchemaStore<Adaptor>::schema(const string& path) const
{
	typename map<string, typename Adaptor::Value>::const_iterator it(d_schemas.find(path));
	if(it == d_schemas.end())
	{
		throw NoSchema(path);
	}
	return it->second;
}

}

JSONSCHEMA_INSTANTIATE_TEMPLATE(JsonSchema::SchemaStore)

