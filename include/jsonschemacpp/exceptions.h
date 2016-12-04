#ifndef JSONSCHEMA_EXCEPTIONS_H
#define JSONSCHEMA_EXCEPTIONS_H

#include <stdexcept>
#include "jsonschemacpp/exceptions.h"
#include "jsonschemacpp/schematypes.h"
#include "jsonschemacpp/utils.h"

namespace JsonSchema
{

class NoSchema : public std::runtime_error
{
public:
	explicit NoSchema(const std::string& what_arg) : std::runtime_error(what_arg) {}
};

class ParseError : public std::runtime_error
{
public:
	explicit ParseError(const std::string& what_arg) : std::runtime_error(what_arg) {}
};

class SchemaError : public std::runtime_error
{
public:
	explicit SchemaError(const Path& schema_path, const std::string& what_arg) :
		std::runtime_error(what_arg + ": " + as_string(schema_path)) {}
};

}

#endif

