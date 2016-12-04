#ifndef JSONSCHEMA_METASCHEMA_H
#define JSONSCHEMA_METASCHEMA_H

#include <string>

namespace JsonSchema
{

struct MetaSchema
{
	struct Draft4
	{
		static const std::string name;
		static const std::string schema;
	};
};

}

#endif

