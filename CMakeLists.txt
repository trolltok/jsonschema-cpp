cmake_minimum_required(VERSION 2.8.5)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
project(jsonschemacpp)
enable_testing()

set(PKG_CONFIG_USE_CMAKE_PREFIX_PATH ON)

find_package(Boost 1.50.0 REQUIRED COMPONENTS regex)
find_package(JsonCPP 1.7.0 REQUIRED)

set(JSONSCHEMA_VERSION_MAJOR 0)
set(JSONSCHEMA_VERSION_MINOR 1)
set(JSONSCHEMA_VERSION_PATCH 0)

set(JSONSCHEMA_VERSION ${JSONSCHEMA_VERSION_MAJOR}.${JSONSCHEMA_VERSION_MINOR}.${JSONSCHEMA_VERSION_PATCH})

message(STATUS "JsonSchema version: ${JSONSCHEMA_VERSION_MAJOR}.${JSONSCHEMA_VERSION_MINOR}.${JSONSCHEMA_VERSION_PATCH}")

set(RUNTIME_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/bin
	CACHE PATH "Install dir for executables")
set(ARCHIVE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib
	CACHE PATH "Install dir for static libraries")
set(LIBRARY_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib
	CACHE PATH "Install dir for shared libraries")
set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include
	CACHE PATH "Install dir for headers")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(tests)

