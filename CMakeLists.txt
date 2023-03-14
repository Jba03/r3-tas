cmake_minimum_required(VERSION 2.8)
project(r3-tas)

set(COMPILE_FLAGS "-fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILE_FLAGS}")

add_compile_definitions(LIBR3TAS_DIR="${CMAKE_BINARY_DIR}")

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
include_directories(game/library/common)
include_directories(game/library/function)
include_directories(game/library/function/hierarchy)
include_directories(game/library/function/script)
include_directories(game/library/structure)
include_directories(game/script)

add_library(r3-tas SHARED
	main.c
	
	common/configuration.c
	
	game/library/function/actor.c
	game/library/function/input.c
	game/library/function/superobject.c
	game/library/function/rnd.c
	game/library/function/object_type.c
	game/library/function/engine.c
	game/library/function/aifunction.c
	game/library/function/dynamics.c
	game/library/function/intersect.c
	game/library/function/ipo.c
	game/library/function/sector.c
	game/library/function/octree.c
	game/library/function/collide_object.c
	game/library/function/intelligence.c
	game/library/function/script.c

	game/interface/export.c
	game/interface/game.c
	game/interface/intfun.c

	graphics/gui/gui.cc
 	#graphics/graphics.c
	#graphics/mesh.c
	graphics/gui/gui.cc

	graphics/gui/imgui/imgui.cpp
	graphics/gui/imgui/imgui_draw.cpp
	graphics/gui/imgui/imgui_tables.cpp
	graphics/gui/imgui/imgui_widgets.cpp
)

set_property(TARGET r3-tas PROPERTY CXX_STANDARD 17)
target_link_libraries(r3-tas ${SDL2_LIBRARIES})
target_link_libraries(r3-tas ${OPENGL_LIBRARIES})