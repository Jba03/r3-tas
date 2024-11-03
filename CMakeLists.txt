cmake_minimum_required(VERSION 2.8)
project(r3-tas)

set(COMPILE_FLAGS "-fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILE_FLAGS}")

add_compile_definitions(LIBR3TAS_DIR="${CMAKE_BINARY_DIR}")

# Include
include_directories(${CMAKE_SOURCE_DIR}/interface)
include_directories(${CMAKE_SOURCE_DIR}/interface/tools)
include_directories(${CMAKE_SOURCE_DIR}/video)
include_directories(${CMAKE_SOURCE_DIR}/video/2d)
include_directories(${CMAKE_SOURCE_DIR}/video/3d)
include_directories(${CMAKE_SOURCE_DIR}/external)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui)
include_directories(${CMAKE_SOURCE_DIR}/external/implot)

# Source files
file(GLOB INTERFACE_SRC ${CMAKE_SOURCE_DIR}/interface/*.cc ${CMAKE_SOURCE_DIR}/interface/tools/*.cc)
file(GLOB IMGUI_SRC ${CMAKE_SOURCE_DIR}/external/imgui/*.cpp)
file(GLOB IMPLOT_SRC ${CMAKE_SOURCE_DIR}/external/implot/*.cpp)

set(VIDEO_SRC
  ${CMAKE_SOURCE_DIR}/video/video.cc
  ${CMAKE_SOURCE_DIR}/video/graphics.mm
  ${CMAKE_SOURCE_DIR}/video/2d/gui.cc
  ${CMAKE_SOURCE_DIR}/video/2d/viewport.cc
  ${CMAKE_SOURCE_DIR}/video/2d/game.cc
  ${CMAKE_SOURCE_DIR}/video/2d/rng.cc
  ${CMAKE_SOURCE_DIR}/video/2d/timer.cc
  ${CMAKE_SOURCE_DIR}/video/2d/marker.cc
  ${CMAKE_SOURCE_DIR}/video/2d/explorer.cc
  ${CMAKE_SOURCE_DIR}/video/2d/common.cc
  ${CMAKE_SOURCE_DIR}/video/2d/input.cc
  ${CMAKE_SOURCE_DIR}/video/2d/cinematic.cc
  ${CMAKE_SOURCE_DIR}/video/2d/ai.cc
  ${CMAKE_SOURCE_DIR}/video/2d/superobject.cc
  #${CMAKE_SOURCE_DIR}/video/2d/dynamics.cc
  ${CMAKE_SOURCE_DIR}/video/2d/hierarchy.cc
  ${CMAKE_SOURCE_DIR}/video/2d/configuration.cc
  ${CMAKE_SOURCE_DIR}/video/2d/bruteforce.cc
  ${CMAKE_SOURCE_DIR}/video/2d/popup.cc
  ${CMAKE_SOURCE_DIR}/video/2d/memory-editor.cc
  ${CMAKE_SOURCE_DIR}/video/2d/run-manager.cc
  ${CMAKE_SOURCE_DIR}/video/3d/metal.mm
)

add_library(r3-tas SHARED ${INTERFACE_SRC} ${VIDEO_SRC} ${IMGUI_SRC} ${IMPLOT_SRC})
target_link_libraries(r3-tas PRIVATE "-framework Cocoa" "-framework Metal")
target_compile_options(r3-tas PRIVATE -w -std=c++20 -O3 -fPIC -fbracket-depth=1024 -DCPA_VERSION=CPA_VERSION_R3_GCN)
