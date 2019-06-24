cmake_minimum_required(VERSION 2.6)
project(plugins)

add_subdirectory(cg_fltk)
add_subdirectory(crg_grid)
add_subdirectory(crg_stereo_view)
add_subdirectory(cmi_io)
add_subdirectory(cg_icons)
add_subdirectory(cg_ext)
add_subdirectory(crg_light)

if(BUILD_EXAMPLE_PLUGIN)
	add_subdirectory(examples)
endif()

add_subdirectory(crg_vr_view)
add_subdirectory(openvr_driver)
add_subdirectory(vr_test)
 
add_custom_target(plugins)

if(BUILD_EXAMPLE_PLUGIN)
	add_dependencies(plugins cg_fltk crg_grid crg_stereo_view cmi_io cg_icons cg_ext crg_light openvr_driver crg_vr_view vr_test examples)
else()
	add_dependencies(plugins cg_fltk crg_grid crg_stereo_view cmi_io cg_icons cg_ext crg_light openvr_driver crg_vr_view vr_test)
endif()

set_target_properties(plugins PROPERTIES FOLDER "${FOLDER_NAME_TOPLEVEL}")



