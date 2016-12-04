# Find JsonCpp. Sets:
# JsonCPP_FOUND - System has JsonCPP
# JsonCPP_INCLUDE_DIRS - Include directories
# JsonCPP_LIBRARIES - Libraries

include(LibFindMacros)

libfind_pkg_check_modules(JsonCPP_PKGCONF jsoncpp)

find_path(JsonCPP_INCLUDE_DIR
	NAMES json/json.h
	PATHS ${JsonCPP_PKGCONF_INCLUDE_DIRS}
	)

find_library(JsonCPP_LIBRARY
	NAMES jsoncpp
	PATHS ${JsonCPP_PKGCONF_LIBRARY_DIRS}
	)

if(NOT JsonCPP_VERSION) # Version not found by pkg-config
	libfind_version_header(JsonCPP json/version.h JSONCPP_VERSION_STRING)
endif()

set(JsonCPP_PROCESS_INCLUDES JsonCPP_INCLUDE_DIR)
set(JsonCPP_PROCESS_LIBS JsonCPP_LIBRARY)
libfind_process(JsonCPP)

if(JsonCPP_FOUND)
	if(NOT JsonCPP_FIND_QUIETLY)
		message(STATUS "Found JsonCPP headers: ${JsonCPP_INCLUDE_DIRS}")
		message(STATUS "Found JsonCPP libraries: ${JsonCPP_LIBRARIES}")
	endif()
endif()

