cmake_minimum_required(VERSION 2.8)
project(r3-tas)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})

set(COMPILE_FLAGS "-fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILE_FLAGS}")

add_compile_definitions(LIBR3TAS_DIR="${CMAKE_BINARY_DIR}")

include_directories(${CMAKE_SOURCE_DIR})
include_directories(common)
include_directories(common/console)
include_directories(PRIVATE common/math)
include_directories(common/type)
include_directories(common/util)
include_directories(graphics)
include_directories(graphics/gui)
include_directories(graphics/gui/imgui)
include_directories(game/interface)
include_directories(game/library/common)
include_directories(game/library/function)
include_directories(game/library/function/hierarchy)
include_directories(game/library/function/script)
include_directories(game/library/structure)
include_directories(game/script)

add_library(r3tas SHARED
	main.c
	
	common/configuration.c
	
	game/interface/export.c
	game/interface/game.c
	game/interface/intfun.c

	graphics/gui/gui.cc

        graphics/camera.c
 	#graphics/graphics.c
	#graphics/mesh.c
	graphics/gui/gui.cc

	graphics/gui/imgui/imgui.cpp
	graphics/gui/imgui/imgui_draw.cpp
	graphics/gui/imgui/imgui_tables.cpp
	graphics/gui/imgui/imgui_widgets.cpp
)

set_property(TARGET r3tas PROPERTY CXX_STANDARD 17)
target_link_libraries(r3tas ${SDL2_LIBRARIES})
target_link_libraries(r3tas ${OPENGL_LIBRARIES})