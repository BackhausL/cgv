cmake_minimum_required(VERSION 2.6)
project(vr_test)

# -----------------------------------------------------------------------------
## CGV Framework ##
# The CGV framework is needed
find_package(cgv 
	COMPONENTS utils type data base math media gui render
	HINTS ${CGV_DIR}
)

# -----------------------------------------------------------------------------
## Dependencies from CGV Framework ##
# Libraries
cgv_find_package(cgv_gl)

# VR
cgv_find_package(vr)
cgv_find_package(cg_vr)
# Gamepad
cgv_find_package(gamepad)
cgv_find_package(cg_gamepad)

# -----------------------------------------------------------------------------
## Plugins from CGV Framework ##
# plugin libraries - cgv/plugins
cgv_find_package(cmi_io)
cgv_find_package(cg_fltk)
cgv_find_package(crg_grid)
cgv_find_package(cg_ext)
cgv_find_package(crg_vr_view_lib)
cgv_find_package(openvr_driver)

set(PLUGINS
	cmi_io
    cg_fltk
	crg_grid
	cg_ext
	crg_vr_view_lib
	vr_emulator
	openvr_driver
)

# -----------------------------------------------------------------------------
# Source files
set(SOURCES
	vr_test.cxx)

file( GLOB_RECURSE SHADERS RELATIVE "${CGV_DIR}/libs/cgv_gl/glsl/" "*.gl*")

include_directories(
	# source directory
	${CMAKE_SOURCE_DIR}
	# CGV framework Components
	${cgv_INCLUDE_DIRS} # all <cgv/...>
	${cgv_gl_INCLUDE_DIRS} # e.g. <cgv_gl/gl/mesh_render_info.h>
	# gamepad
	${gamepad_INCLUDE_DIRS} # e.g. <gamepad/gamepad.h>
	${cg_gamepad_INCLUDE_DIRS} # e.g. <cg_gamepad/gamepad_server.h>
	# vr
	${vr_INCLUDE_DIRS} # e.g. <vr/vr_driver.h>
	${cg_vr_INCLUDE_DIRS} # e.g. <cg_vr/vr_server.h>
	# vr_view_interactor
	${crg_vr_view_lib_INCLUDE_DIRS} # e.g. <vr_view_interactor.h>
)

cgv_add_module(vr_test 
	SOURCES ${SOURCES}
	SHADERS ${SHADERS}
)

target_link_libraries(vr_test 
	# CGV framework Components
	${cgv_LIBRARIES}
	${cgv_gl_LIBRARIES} 
	# gamepad
	${gamepad_LIBRARIES}
	${cg_gamepad_LIBRARIES}
	# vr
	${vr_LIBRARIES}
	${cg_vr_LIBRARIES}
	# vr_view_interactor
	${crg_vr_view_lib_LIBRARIES}
)

# set export definitions
cgv_add_export_definitions(vr_test VR_TEST_EXPORTS)

# If building on Windows - add config for visual studio
if (WIN32)
	cgv_get_viewer_locations(VIEWER_EXE VIEWER_DEBUG_EXE)
	set_target_properties(vr_test PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "plugin:cmi_io plugin:cg_fltk type(shader_config):shader_path='${CGV_DIR}/libs/cgv_gl/glsl' plugin:crg_grid plugin:cg_ext plugin:crg_vr_view_lib plugin:vr_emulator plugin:openvr_driver plugin:vr_test")
	set_target_properties(vr_test PROPERTIES VS_DEBUGGER_COMMAND $<IF:$<CONFIG:Debug>,${VIEWER_DEBUG_EXE},${VIEWER_EXE}>)
endif()

cgv_write_find_file(vr_test)

