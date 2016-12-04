#ifndef JSONSCHEMA_DRAFT4VALIDATOR_H
#define JSONSCHEMA_DRAFT4VALIDATOR_H

#include <functional>
#include <map>
#include <memory>
#include <regex>
#include <string>
#include <vector>
#include "jsonschemacpp/schemastore.h"
#include "jsonschemacpp/schematypes.h"
#include "jsonschemacpp/validatorbase.h"

namespace JsonSchema
{

template<class Adaptor>
class Draft4Validator : public ValidatorBase<Adaptor>
{
public:
	Draft4Validator();

private:
	static const typename ValidatorBase<Adaptor>::CheckFunctionMap draft4_check_functions;

	// 5.1 Numeric validators
	bool check_multipleof(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
			typename Adaptor::ConstValue schema, bool log_errors);
	bool check_maximum(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
			typename Adaptor::ConstValue schema, bool log_errors);
	bool check_minimum(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
			typename Adaptor::ConstValue schema, bool log_errors);

	// 5.2 String validators
	bool check_maxlength(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
			typename Adaptor::ConstValue schema, bool log_errors);
	bool check_minlength(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
			typename Adaptor::ConstValue schema, bool log_errors);
	bool check_pattern(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
			typename Adaptor::ConstValue schema, bool log_errors);

	// 5.3 Array validators
	bool check_items(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
			typename Adaptor::ConstValue schema, bool log_errors);
	bool check_additionalitems(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
			typename Adaptor::ConstValue schema, bool log_errors);
	bool check_maxitems(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
			typename Adaptor::ConstValue schema, bool log_errors);
	bool check_minitems(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
			typename Adaptor::ConstValue schema, bool log_errors);
	bool check_uniqueitems(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
			typename Adaptor::ConstValue schema, bool log_errors);

	// 5.4 Object validators
	bool check_maxproperties(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
			typename Adaptor::ConstValue schema, bool log_errors);
	bool check_minproperties(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
			typename Adaptor::ConstValue schema, bool log_errors);
	bool check_required(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
			typename Adaptor::ConstValue schema, bool log_errors);
	bool check_properties(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
			typename Adaptor::ConstValue schema, bool log_errors);
	bool check_patternproperties(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
			typename Adaptor::ConstValue schema, bool log_errors);
	bool check_additionalproperties(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
			typename Adaptor::ConstValue schema, bool log_errors);
	bool check_dependencies(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
			typename Adaptor::ConstValue schema, bool log_errors);

	// 5.5 Any type validators
	bool check_enum(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
			typename Adaptor::ConstValue schema, bool log_errors);
	bool check_type(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
			typename Adaptor::ConstValue schema, bool log_errors);
	bool check_allof(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
			typename Adaptor::ConstValue schema, bool log_errors);
	bool check_anyof(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
			typename Adaptor::ConstValue schema, bool log_errors);
	bool check_oneof(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
			typename Adaptor::ConstValue schema, bool log_errors);
	bool check_not(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
			typename Adaptor::ConstValue schema, bool log_errors);

	// 7 Format validator
	//bool check_format(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
	//		typename Adaptor::ConstValue schema, std::vector<ValidationError>* errors) const;

	// Reference validator
	bool check_ref(typename Adaptor::ConstValue instance, typename Adaptor::ConstValue constraint,
			typename Adaptor::ConstValue schema, bool log_errors);
};

}

#endif

