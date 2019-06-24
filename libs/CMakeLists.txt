cmake_minimum_required(VERSION 2.6)
project(libs)

add_subdirectory(glsu)
add_subdirectory(delaunay)

add_subdirectory(cgv_reflect_types)
add_subdirectory(cgv_gl)
add_subdirectory(plot)
add_subdirectory(point_cloud)

add_subdirectory(vr)
add_subdirectory(cg_vr)
add_subdirectory(gamepad)
add_subdirectory(cg_gamepad)

add_custom_target(libs)

add_dependencies(libs cgv_gl glsu plot cgv_reflect_types delaunay vr gamepad cg_vr cg_gamepad)

set_target_properties(libs PROPERTIES FOLDER "${FOLDER_NAME_LIBS}")

