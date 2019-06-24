cmake_minimum_required(VERSION 3.0.2)
project(openvr_driver)

# -----------------------------------------------------------------------------
## SYSTEM PROPERTIES ##

# If not set, determines the running platform architecture.
if(NOT PLATFORM)
  if(CMAKE_SIZEOF_VOID_P MATCHES 8)
    set(PLATFORM 64)
  else()
    set(PLATFORM 32)
  endif()
endif()
message(STATUS "Compilation set for ${PLATFORM}bits architectures.")

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(ARCH_TARGET linux64)
	set(API_PATH_SUFFIXES linux${PLATFORM} )

	if(${PLATFORM} MATCHES 32)
		message(WARNING "OpenVR x86 binaries not provided on GNU/Linux.")
	endif()

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(CMAKE_MACOSX_RPATH 0)
  add_definitions(-DOSX -DPOSIX)
  set(ARCH_TARGET osx32)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set(ARCH_TARGET win${PLATFORM})
  set(API_PATH_SUFFIXES win${PLATFORM} )
endif()

message(STATUS "Searching for openvr_api in subdirectory: ${API_PATH_SUFFIXES}")

# -----------------------------------------------------------------------------
## CGV Framework ##
# The CGV framework is needed
# only for cgv_ commands
find_package(cgv COMPONENTS HINTS ${CGV_DIR})

# -----------------------------------------------------------------------------
## Dependencies from CGV Framework ##
cgv_find_package(vr)

# -----------------------------------------------------------------------------
## LIBRARIES ##
cgv_find_package(GLEW REQUIRED)
cgv_find_package(OpenGL REQUIRED)

## OpenVR API path
find_library(OPENVR_LIBRARIES
  NAMES
    openvr_api
  PATHS
	  ${CMAKE_CURRENT_SOURCE_DIR}
      ${CMAKE_CURRENT_SOURCE_DIR}/../../bin
  PATH_SUFFIXES
	  ${API_PATH_SUFFIXES}	# Use the correct path suffix according to architecture, e.g win32, win64, linux64
    NO_DEFAULT_PATH
    NO_CMAKE_FIND_ROOT_PATH
)
set(OPENVR_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# -----------------------------------------------------------------------------
## SHARED SOURCES ##
set(HEADERS 
	openvr.h
	openvr_kit.h )

set(SOURCES
	openvr_driver.cxx
	openvr_kit.cxx)


include_directories(
	${GLEW_INCLUDE_PATH}
	${OPENGL_INCLUDE_DIR}
	${OPENVR_INCLUDE_DIR}
	${vr_INCLUDE_DIR}
)

cgv_add_module(openvr_driver ${SOURCES} ${HEADERS})

add_dependencies(openvr_driver vr)

cgv_add_export_definitions(openvr_driver OPENVR_DRIVER_EXPORTS)

target_link_libraries(openvr_driver 
	${GLEW_LIBRARIES} 
	${OPENGL_LIBRARIES}
	${OPENVR_LIBRARIES}
	${vr_LIBRARIES}
)

cgv_write_find_file(openvr_driver) 

