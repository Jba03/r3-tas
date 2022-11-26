cmake_minimum_required(VERSION 2.8)
project(r3-tas)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})

set(COMPILE_FLAGS "-fPIC -dynamiclib")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILE_FLAGS}")

include_directories(${CMAKE_SOURCE_DIR})
include_directories(common)
include_directories(graphics)
include_directories(structure/collide)
include_directories(structure/global)
include_directories(structure/math)
include_directories(structure/object)
include_directories(structure/ai)
include_directories(predict)
include_directories(imgui)

add_library(r3tas SHARED
	main.c
	common.c
	configuration.c
	logging.c
	memory.c
	stream.c
	
	common/array.c
	
	graphics/camera.c
	graphics/graphics.c
	graphics/glmesh.c
	graphics/gui.cc
		
	structure/global/engine.c
	structure/global/engine_timer.c
	structure/global/fix.c
	structure/global/lvl.c
	structure/global/stdgame.c
	structure/global/superobject_links.c
	structure/global/dynamics.c

	structure/object/actor.c
	structure/object/physical_object.c
	structure/object/superobject.c

	structure/collide/collide.c
	structure/collide/mesh.c
	structure/collide/octree.c

	structure/ai/brain.c
	structure/ai/mind.c
	structure/ai/dsg.c
	
	structure/math/vector3.c
	structure/math/vector4.c
	structure/math/matrix3.c
	structure/math/matrix4.c
	structure/math/transform.c

	predict/predict.c

	imgui/imgui.cpp
	imgui/imgui_draw.cpp
	imgui/imgui_tables.cpp
	imgui/imgui_widgets.cpp
)

set_property(TARGET r3tas PROPERTY CXX_STANDARD 17)
target_link_libraries(r3tas ${SDL2_LIBRARIES})
target_link_libraries(r3tas ${OPENGL_LIBRARIES})