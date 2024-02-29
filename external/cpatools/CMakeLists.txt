cmake_minimum_required(VERSION 2.8)
project(cpatools)

# Target version.
# Must be one of the CPA_VERSION_# directives found in config.hh
add_definitions(-DCPA_VERSION=${TARGET_VERSION})

include_directories(${CMAKE_SOURCE_DIR})
file(GLOB cpatools "${CMAKE_SOURCE_DIR}/*.cc")

set(CMAKE_CXX_FLAGS "-Wfatal-errors -O3")
add_library(cpatools SHARED ${cpatools})
set_property(TARGET cpatools PROPERTY CXX_STANDARD 23)