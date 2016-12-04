set(INCLUDE_FILES
	jsonschemacpp/adaptorjsoncpp.h
	jsonschemacpp/adaptors.h
	jsonschemacpp/draft4validator.h
	jsonschemacpp/exceptions.h
	jsonschemacpp/jsonschema.h
	jsonschemacpp/metaschema.h
	jsonschemacpp/schemastore.h
	jsonschemacpp/schematypes.h
	jsonschemacpp/utils.h
	jsonschemacpp/validatorbase.h
	${CMAKE_CURRENT_BINARY_DIR}/jsonschemacpp/version.h
	)
install(FILES ${INCLUDE_FILES} DESTINATION ${INCLUDE_INSTALL_DIR}/jsonschemacpp)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/jsonschemacpp/version.h.in"
	"${CMAKE_CURRENT_BINARY_DIR}/jsonschemacpp/version.h")

