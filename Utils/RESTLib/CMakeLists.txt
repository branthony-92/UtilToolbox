cmake_minimum_required(VERSION 3.9)
project(RESTLib)

# Version info
set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)
set (VERSION_BUILD 0)
set (VERSION_REV   0)
set (VERSION_STRING ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_BUILD}-${VERSION_REV})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
add_compile_options(/W4 /bigobj)

# include the current directory
include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/../3rdParty/boost/include/")
include_directories("${PROJECT_SOURCE_DIR}/../3rdParty/boost/include/")
include_directories("${PROJECT_SOURCE_DIR}/../json-develop/include")
include_directories("${PROJECT_SOURCE_DIR}/../ActionProcessor")

# set the sources
file(GLOB ROOT_HEADERS "*.h")
file(GLOB ROOT_SOURCES "*.cpp")

list(REMOVE_ITEM ROOT_SOURCES "${PROJECT_SOURCE_DIR}/RESTRequest.cpp")
list(REMOVE_ITEM ROOT_SOURCES "${PROJECT_SOURCE_DIR}/RESTClient.cpp")
list(REMOVE_ITEM ROOT_HEADERS "${PROJECT_SOURCE_DIR}/RESTRequest.h")
list(REMOVE_ITEM ROOT_HEADERS "${PROJECT_SOURCE_DIR}/RESTClient.h")

source_group("Header Files\\${ROOT_HEADERS})
source_group("Source Files\\${ROOT_SOURCES})

set(SOURCES ${ROOT_HEADERS} ${ROOT_SOURCES})
add_library(restlib ${SOURCES})

add_definitions(-include "${PROJECT_SOURCE_DIR}/stdafx.h")
#add_library(objectlib OBJECT ${ROOT_SOURCES})
#add_library(restlib STATIC $<TARGET_OBJECTS:objectlib>)

# set_target_properties(restlib PROPERTIES ${VERSION_STRING})
# Since this is static we don't need to link anything else in