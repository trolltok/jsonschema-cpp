#ifndef JSONSCHEMA_ADAPTORS_H
#define JSONSCHEMA_ADAPTORS_H

#include "jsonschemacpp/adaptorjsoncpp.h"

#define JSONSCHEMA_INSTANTIATE_TEMPLATE(CLASS) \
	template class CLASS<JsonSchema::Adaptors::JsonCppAdaptor>;

#endif

