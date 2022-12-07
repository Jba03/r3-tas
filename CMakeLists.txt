cmake_minimum_required(VERSION 2.8)
project(r3-tas)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})

set(COMPILE_FLAGS "-fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILE_FLAGS}")

include_directories(${CMAKE_SOURCE_DIR})
include_directories(common)
include_directories(graphics)
include_directories(game/collide)
include_directories(game/global)
include_directories(game/graph)
include_directories(game/math)
include_directories(game/object)
include_directories(game/ai)
include_directories(game/predict)
include_directories(game)
include_directories(imgui)

add_library(r3tas SHARED
	main.c
	
	common/array.c
	common/configuration.c
	common/logging.c
	common/memory.c
	common/stream.c
	common/vector2.c
	common/vector3.c
	common/vector4.c
	common/matrix3.c
	common/matrix4.c
	
	graphics/camera.c
	graphics/graphics.c
	graphics/glmesh.c
	graphics/gui.cc
		
	game/global/engine.c
	game/global/engine_timer.c
	game/global/fix.c
	game/global/lvl.c
	game/global/random.c
	game/global.c

	game/object/actor.c
	game/object/physical_object.c
	game/object/superobject.c
	game/object/superobject_links.c
	game/object/dynamics.c
	game/object/transform.c
	game/object/stdgame.c

	game/graph/graph.c
	game/graph/graphnode.c
	game/graph/waypoint.c
	game/graph/msway.c

	game/collide/collide.c
	game/collide/mesh.c
	game/collide/octree.c

	game/ai/brain.c
	game/ai/mind.c
	game/ai/dsg.c
	game/ai/intelligence.c
	game/ai/behavior.c
	game/ai/script.c	
	game/ai/interpreter.c

	game/predict/predict.c

	imgui/imgui.cpp
	imgui/imgui_draw.cpp
	imgui/imgui_tables.cpp
	imgui/imgui_widgets.cpp
)

set_property(TARGET r3tas PROPERTY CXX_STANDARD 17)
target_link_libraries(r3tas ${SDL2_LIBRARIES})
target_link_libraries(r3tas ${OPENGL_LIBRARIES})