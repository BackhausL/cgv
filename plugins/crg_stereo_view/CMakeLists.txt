cmake_minimum_required(VERSION 2.6)
project(crg_stereo_view)


# The CGV framework is needed
find_package(cgv 
	COMPONENTS render gui utils type reflect data base signal
	HINTS $ENV{CGV_DIR}
	)

cgv_find_package(glsu)
cgv_find_package(cgv_gl)

set(HEADERS 
	stereo_view_interactor.h)

set(SOURCES
	stereo_view_interactor.cxx)

include_directories(
	${cgv_INCLUDE_DIRS}
	${glsu_INCLUDE_DIRS}
	${cgv_gl_INCLUDE_DIRS})


cgv_add_module(crg_stereo_view ${SOURCES} ${HEADERS})

cgv_add_export_definitions(crg_stereo_view CGV_RENDER_STEREO_VIEW)

if(WIN32)
	cgv_add_library(crg_stereo_view_lib 
		SOURCES ${SOURCES} 
		PUBLIC_HEADERS ${HEADERS}
		DEFINITIONS UNICODE _UNICODE
		SHARED_DEFINITIONS CGV_RENDER_STEREO_VIEW_EXPORTS
		STATIC_DEFINITIONS NO_STEREO_VIEW_INTERACTOR)
else()
	cgv_add_library(crg_stereo_view_lib 
		SOURCES ${SOURCES} 
		PUBLIC_HEADERS ${HEADERS}
		SHARED_DEFINITIONS CGV_RENDER_STEREO_VIEW_EXPORTS
		STATIC_DEFINITIONS NO_STEREO_VIEW_INTERACTOR)
endif()

target_link_libraries(crg_stereo_view  ${cgv_LIBRARIES} ${glsu_LIBRARIES} ${cgv_gl_LIBRARIES})
target_link_libraries(crg_stereo_view_lib  ${cgv_LIBRARIES} ${glsu_LIBRARIES} ${cgv_gl_LIBRARIES})

#set_target_properties(crg_stereo_view PROPERTIES FOLDER "${FOLDER_NAME_PLUGINS}")

cgv_write_find_file(crg_stereo_view)
cgv_write_find_file(crg_stereo_view_lib)
