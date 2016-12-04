include_directories(${Boost_INCLUDE_DIRS})
include_directories(${JsonCPP_INCLUDE_DIR})
include_directories(${jsonschemacpp_SOURCE_DIR}/include)
include_directories(${CMAKE_BINARY_DIR}/include)

add_definitions(-DJSONSCHEMA_TEST_SUITE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/suite")
add_executable(test_jsonschema main.cpp)
target_link_libraries(test_jsonschema jsonschemacpp_lib ${JsonCPP_LIBRARIES} ${Boost_LIBRARIES})
set_target_properties(test_jsonschema PROPERTIES OUTPUT_NAME test_jsonschema)
add_test(NAME test_jsonschema COMMAND test_jsonschema)

