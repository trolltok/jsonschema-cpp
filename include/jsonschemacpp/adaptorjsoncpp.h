#ifndef JSONSCHEMA_ADAPTORJSONCPP_H
#define JSONSCHEMA_ADAPTORJSONCPP_H

#include <cassert>
#include <iterator>
#include <string>
#include <utility>
#include <json/json.h>

namespace JsonSchema
{
namespace Adaptors
{

struct JsonCppAdaptor
{
	class Value;
	class ConstValue;
	class ConstValueIterator;

	class Value
	{
	public:
		Value() {}
		Value(const Json::Value& v) : d_v(v) {}
		Value(Value&& other) = default;
		explicit Value(const ConstValue& other) : d_v(*other.d_v) {}

		bool parse(const std::string& doc) { Json::Reader reader; return reader.parse(doc, d_v); }

	private:
		Json::Value d_v;
		friend class ConstValue;
	};

	class ConstValue
	{
	public:
		using const_iterator = ConstValueIterator;

		ConstValue() : d_v(0) {}
		ConstValue(const Json::Value& v) : d_v(&v) {}
		ConstValue(const Value& other) : d_v(&other.d_v) {}

		operator bool() const { return d_v != 0; }

		bool is_array() const { assert(d_v); return d_v->isArray() && !d_v->isNull(); }
		bool is_bool() const { assert(d_v); return d_v->isBool(); }
		bool is_integral() const { assert(d_v); return d_v->isIntegral() && !d_v->isBool(); }
		bool is_numeric() const { assert(d_v); return d_v->isNumeric() && !d_v->isBool(); }
		bool is_null() const { assert(d_v); return d_v->isNull(); }
		bool is_object() const { assert(d_v); return d_v->isObject() && !d_v->isNull(); }
		bool is_string() const { assert(d_v); return d_v->isString(); }
		bool is_valid_index(unsigned int index) const { assert(d_v); return d_v->isValidIndex(index); }
		bool is_member(const std::string& member) const { assert(d_v); return d_v->isMember(member); }

		bool as_bool() const { assert(d_v); return d_v->asBool(); }
		double as_double() const { assert(d_v); return d_v->asDouble(); }
		int as_int() const { assert(d_v); return d_v->asInt(); }
		unsigned int as_uint() const { assert(d_v); return d_v->asUInt(); }
		std::string as_string() const { assert(d_v); return std::move(d_v->asString()); }

		unsigned int size() const { assert(d_v); return d_v->size(); }

		bool operator==(const ConstValue& other) const { assert(d_v); assert(other.d_v); return (*d_v) == (*other.d_v); }

		ConstValue operator[](unsigned int index) const { assert(d_v); return (*d_v)[index]; }
		ConstValue operator[](const std::string& name) const { assert(d_v); return (*d_v)[name]; }
		ConstValue operator[](const char* name) const { assert(d_v); return (*d_v)[name]; }

		ConstValueIterator begin() const { assert(d_v); return d_v->begin(); }
		ConstValueIterator end() const { assert(d_v); return d_v->end(); }

	private:
		const Json::Value* d_v;
		friend class Value;
	};

	class ConstValueIterator : public std::iterator<std::forward_iterator_tag, ConstValue>
	{
	public:
		ConstValueIterator() {}
		ConstValueIterator(const Json::Value::const_iterator& it) : d_it(it) {}
		ConstValueIterator& operator++() { d_it++; return *this; }
		ConstValueIterator operator++(int) { ConstValueIterator tmp(*this); d_it++; return tmp; }
		bool operator==(const ConstValueIterator& other) const { return d_it == other.d_it; }
		bool operator!=(const ConstValueIterator& other) const { return d_it != other.d_it; }
		ConstValue operator*() const { return *d_it; }
		ConstValue operator->() const { return *d_it; }
		void swap(ConstValueIterator& other) { std::swap(d_it, other.d_it); }
		std::string name() const { return std::move(d_it.name()); }
	private:
		Json::Value::const_iterator d_it;
	};
};

}
}

#endif

