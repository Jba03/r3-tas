cmake_minimum_required(VERSION 2.8)
project(r3-tas)

set(COMPILE_FLAGS "-fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILE_FLAGS}")

add_compile_definitions(LIBR3TAS_DIR="${CMAKE_BINARY_DIR}")

# Include
include_directories(${CMAKE_SOURCE_DIR})
include_directories(common)
include_directories(common/console)
include_directories(common/math)
include_directories(common/type)
include_directories(common/util)
include_directories(graphics)
include_directories(graphics/gui)
include_directories(graphics/gui/imgui)
include_directories(game/interface)
include_directories(game/interface/analyse)
include_directories(game/interface/xray)

include_directories(game/library)
include_directories(game/library/common)
include_directories(game/library/function)
include_directories(game/library/function/hierarchy)
include_directories(game/library/function/script)
include_directories(game/library/structure)
include_directories(game/script)

# Source files
file(GLOB common "common/**.c")
file(GLOB library "game/library/function/*.c")
file(GLOB interface "game/interface/**.c")
file(GLOB xray "game/interface/xray/**.c")
file(GLOB gui "graphics/gui/gui.cc")
file(GLOB imgui "graphics/gui/imgui/*.cpp")

add_library(r3-tas SHARED main.c ${common} ${library} ${interface} ${xray} ${gui} ${imgui})

set_property(TARGET r3-tas PROPERTY CXX_STANDARD 17)
target_link_libraries(r3-tas ${SDL2_LIBRARIES})
target_link_libraries(r3-tas ${OPENGL_LIBRARIES})