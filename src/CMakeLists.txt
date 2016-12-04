set(jsonschemacpp_SOURCES
	draft4validator.cc
	metaschema.cc
	schemastore.cc
	utils.cc
	validatorbase.cc
	)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${JsonCPP_INCLUDE_DIR})
include_directories(${jsonschemacpp_SOURCE_DIR}/include)
include_directories(${CMAKE_BINARY_DIR}/include)

add_library(jsonschemacpp_lib STATIC ${jsonschemacpp_SOURCES})

set_target_properties(jsonschemacpp_lib PROPERTIES VERSION ${JSONSCHEMA_VERSION} SOVERSION ${JSONSCHEMA_VERSION_MAJOR})
set_target_properties(jsonschemacpp_lib PROPERTIES OUTPUT_NAME jsonschemacpp)

install(TARGETS jsonschemacpp_lib ${INSTALL_EXPORT}
	RUNTIME DESTINATION ${RUNTIME_INSTALL_DIR}
	LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
	ARCHIVE DESTINATION ${ARCHIVE_INSTALL_DIR})


