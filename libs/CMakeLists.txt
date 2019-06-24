cmake_minimum_required(VERSION 2.6)
project(libs)

add_subdirectory(glsu)
add_subdirectory(delaunay)

add_subdirectory(cgv_reflect_types)
add_subdirectory(cgv_gl)
add_subdirectory(plot)
add_subdirectory(point_cloud)

add_subdirectory(gamepad)
add_subdirectory(cg_gamepad)

if(BUILD_VR_SUPPORT)
	add_subdirectory(vr)
	add_subdirectory(cg_vr)
endif()


add_custom_target(libs)

if(BUILD_VR_SUPPORT)
	add_dependencies(libs cgv_gl glsu plot cgv_reflect_types delaunay gamepad cg_gamepad vr cg_vr)
else()
	add_dependencies(libs cgv_gl glsu plot cgv_reflect_types delaunay gamepad cg_gamepad)
endif()

set_target_properties(libs PROPERTIES FOLDER "${FOLDER_NAME_LIBS}")

