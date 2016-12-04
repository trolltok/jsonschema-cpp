#include <fstream>
#include <iostream>
#include <json/json.h>
#include <jsonschemacpp/jsonschema.h>

using namespace std;

static const char* suite_path = JSONSCHEMA_TEST_SUITE_PATH;
static constexpr unsigned int num_tests = 28;
static const char* tests[num_tests] = {
	"additionalItems.json",
	"additionalProperties.json",
	"allOf.json",
	"anyOf.json",
	"default.json",
	"definitions.json",
	"dependencies.json",
	"enum.json",
	"items.json",
	"maxItems.json",
	"maxLength.json",
	"maxProperties.json",
	"maximum.json",
	"minItems.json",
	"minLength.json",
	"minProperties.json",
	"minimum.json",
	"multipleOf.json",
	"not.json",
	"oneOf.json",
	"pattern.json",
	"patternProperties.json",
	"properties.json",
	"ref.json",
	"refRemote.json",
	"required.json",
	"type.json",
	"uniqueItems.json"
};

static bool verbose = true;

static bool run_test(const Json::Value& test_desc);

int main(int argc, char** argv)
{
	bool success = true;
	for(unsigned int test = 0; test < num_tests; test++)
	{
		string fn(suite_path);
		fn += "/";
		fn += tests[test];
		ifstream ifs(fn);
		Json::Reader reader;
		Json::Value test_desc;
		if(!reader.parse(ifs, test_desc))
		{
			cerr << "Failed reading test set " << tests[test] << endl;
			continue;
		}

		cerr << "Run test set " << tests[test] << endl;
		if(!run_test(test_desc))
		{
			success = false;
			cerr << "  *** Failed test set " << tests[test] << endl;
		}
	}

	return success ? 0 : 1;
}

bool run_test(const Json::Value& test_desc)
{
	bool all_success = true;
	JsonSchema::Draft4Validator<JsonSchema::Adaptors::JsonCppAdaptor> validator;
	for(const Json::Value& test_group : test_desc)
	{
		if(verbose)
			cerr << "  Run set " << test_group["description"].asString() << endl;
		const Json::Value& schema(test_group["schema"]);
		for(const Json::Value& test_case : test_group["tests"])
		{
			if(verbose)
				cerr << "    test case " << test_case["description"].asString() << ": ";
			const Json::Value& instance(test_case["data"]);
			bool expected(test_case["valid"].asBool());
			bool success;
			try
			{
				success = validator.validate(instance, schema);
			}
			catch(const std::runtime_error& e)
			{
				cerr << "Exception: " << e.what() << endl;
				throw;
			}

			if(success == expected)
			{
				if(verbose)
					cerr << "Success";
			}
			else
			{
				if(verbose)
					cerr << "******** FAILURE ********";
				all_success = false;
			}
			if(verbose)
				cerr << endl;
		}
	}
	return all_success;
}

