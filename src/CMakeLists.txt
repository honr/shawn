#==================================
#
# SHAWN build system
#
# Main input file for CMake
# 
#==================================

#=============
# Configure CMake
#=============

cmake_minimum_required(VERSION 2.4)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

#=============
# Set the project's name
#
# This makes the following cmake vars available:
#   ${SHAWN_SOURCE_DIR}
#   ${SHAWN_BINARY_DIR}
#
#=============

	project( SHAWN )

#=============
# Options
#=============

	OPTION(OPT_ENABLE_CGAL "Enable CGAL library support" OFF)
	OPTION(OPT_ENABLE_CAIRO "Enable Cairo library support" OFF)
	OPTION(OPT_ENABLE_BOOST "Enable boost library support" OFF)
	OPTION(OPT_CONFIGURE_LOGGING "Configure logging system" OFF)
	OPTION(OPT_ENABLE_GLUT "Enable GLUT library used by vis Liveview" OFF)

#=============
# Set the default paths for binary and lib build
#=============

	get_filename_component( EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../buildfiles ABSOLUTE CACHE )
	get_filename_component( LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../buildfiles ABSOLUTE CACHE )

#=============
# Set the default build type to "Release"
#=============

	IF(NOT CMAKE_BUILD_TYPE)
		SET(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
	ENDIF(NOT CMAKE_BUILD_TYPE)

	
#=============
# Collect all source files recursively to be compiled
#=============

	file( GLOB_RECURSE shawnSourcesAppsCPP apps/*.cpp )
	file( GLOB_RECURSE shawnSourcesAppsH apps/*.h )

	file( GLOB_RECURSE shawnSourcesFrontendCPP frontend/*.cpp )
	file( GLOB_RECURSE shawnSourcesFrontendH frontend/*.h )
	
	file( GLOB_RECURSE shawnSourcesSysCPP sys/*.cpp sys/*.c )
	file( GLOB_RECURSE shawnSourcesSysH sys/*.h )
	
	#file( GLOB_RECURSE shawnSourcesConfig shawn_config.h )

	#
	# Collect LEGACYAPPS sources if present
	#
	
	set( LEGACYAPPS_PATH "" CACHE PATH "Path to legacy apps. Leave blank to not use legacy apps." )
	if ( LEGACYAPPS_PATH )
		file( GLOB_RECURSE legacyappsSourcesCPP ${LEGACYAPPS_PATH}/*.cpp )
	endif ( LEGACYAPPS_PATH )

	#
	# Let the user choosse whether to build the shawn-with-main lib
	#

	OPTION(BUILD_LIB_WITH_MAIN "Additionally build a library containing main()" OFF)

#=============
# Add collected source files to common source variables
#=============

	# shawnSources - Sources for executable build

		set( shawnSources ${shawnSources} ${shawnSourcesFrontendCPP} )
		set( shawnSources ${shawnSources} ${shawnSourcesFrontendH} )
		set( shawnSources ${shawnSources} shawn_config.h )

	# shawnSourcesLib - Sources for library build

		set( shawnSourcesLib ${shawnSourcesLib} ${shawnSourcesSysCPP} )
		set( shawnSourcesLib ${shawnSourcesLib} ${shawnSourcesSysH} )

		set( shawnSourcesLib ${shawnSourcesLib} ${shawnSourcesAppsCPP} )
		set( shawnSourcesLib ${shawnSourcesLib} ${shawnSourcesAppsH} )

		set( shawnSourcesLib ${shawnSourcesLib} ${legacyappsSourcesCPP} )

		set( shawnSourcesLib ${shawnSourcesLib} shawn_config.h )

#=============
# Add include and library directories for the compiler/linker
#=============

    include_directories (
        ./
        ${CMAKE_BINARY_DIR}/../buildfiles
    )

	if ( OPT_ENABLE_CGAL )
		set( DEFAULT_CGAL_ROOT )
		set( LIB_PATH_CGAL CACHE PATH "Path to CGAL library" )
		set( INCLUDE_PATH_CGAL CACHE PATH "Path to CGAL includes" )
	        set( INCLUDE_PATH_CGAL_COMPILER_CONFIG CACHE PATH "Path to CGAL compiler config" )
		link_directories( ${LIB_PATH_CGAL} )
		include_directories ( 
			${INCLUDE_PATH_CGAL}
			${INCLUDE_PATH_CGAL_COMPILER_CONFIG}
		)
	endif ( OPT_ENABLE_CGAL )

	if( OPT_ENABLE_BOOST )
		set ( LIB_PATH_BOOST   CACHE PATH "Boost lib path" )
		set ( INCLUDE_PATH_BOOST   CACHE PATH "Boost include path" )
		link_directories( ${LIB_PATH_BOOST} )
		include_directories ( ${INCLUDE_PATH_BOOST} )
	endif( OPT_ENABLE_BOOST )

	if ( OPT_ENABLE_CAIRO )
		set ( DEFAULT_CAIRO_ROOT )
		set ( LIB_PATH_CAIRO   CACHE PATH "Path to CAIRO library" )
		set ( INCLUDE_PATH_CAIRO   CACHE PATH "Path to CAIRO includes" )
		link_directories( ${LIB_PATH_CAIRO} )
		include_directories ( ${INCLUDE_PATH_CAIRO} )
	endif ( OPT_ENABLE_CAIRO )	

	if ( OPT_ENABLE_GLUT )
		set ( LIB_PATH_GLUT   CACHE PATH "Path to GLUT library" )
		set ( INCLUDE_PATH_GLUT   CACHE PATH "Path to GLUT includes" )
		link_directories( ${LIB_PATH_GLUT} )
		include_directories ( ${INCLUDE_PATH_GLUT} )
	endif ( OPT_ENABLE_GLUT )	

#=============
# Configure logging libraries
#=============

	if ( OPT_CONFIGURE_LOGGING )
		set ( LOGGER_TYPE "STDOUT" CACHE STRING "Logger type to use. Choose STDOUT(Default), LOG4CXX or NONE." )

		if ( LOGGER_TYPE STREQUAL "STDOUT" )

		elseif ( LOGGER_TYPE STREQUAL "LOG4CXX" )
			set ( INCLUDE_PATH_LOG4CXX CACHE PATH "Path to LOG4CXX includes" )
			set ( LIB_PATH_LOG4CXX CACHE PATH "Path to LOG4CXX libs" )
			add_definitions( -DLOG4CXX_STATIC -DHAVE_LOG4CXX )
			link_directories ( ${LIB_PATH_LOG4CXX} )
			include_directories ( ${INCLUDE_PATH_LOG4CXX} )
			
		elseif ( LOGGER_TYPE STREQUAL "NONE")
		   add_definitions( -DDISABLE_LOGGING )
		   
		else ( LOGGER_TYPE STREQUAL "STDOUT" )
		   message( FATAL_ERROR "Unsupportet logging library" )
		endif ( LOGGER_TYPE STREQUAL "STDOUT" )

		# --- DEBUG and INFO configuration ---
		if( CMAKE_BUILD_TYPE STREQUAL "Release")

		OPTION(LOG_DEBUG_ON_RELEASE "Output debug messages at release configuration" OFF)
		if( LOG_DEBUG_ON_RELEASE )
			add_definitions( -DLOG_DEBUG_ON_RELEASE )
		endif ( LOG_DEBUG_ON_RELEASE )

		OPTION(LOG_INFO_ON_RELEASE "Output info messages at release configuration" ON)
		if( NOT LOG_INFO_ON_RELEASE )
			add_definitions( -DNLOG_INFO_ON_RELEASE )
		endif ( NOT LOG_INFO_ON_RELEASE )

		endif( CMAKE_BUILD_TYPE STREQUAL "Release" )
		
	endif ( OPT_CONFIGURE_LOGGING )


#=============
# Compiler definitions
#=============

	if( OPT_ENABLE_CGAL )
		add_definitions( -DHAVE_CGAL )
	endif( OPT_ENABLE_CGAL )

	if( OPT_ENABLE_BOOST )
		add_definitions(-DHAVE_BOOST -DHAVE_BOOST_REGEX )
	endif( OPT_ENABLE_BOOST )

	if( OPT_ENABLE_CAIRO )
		add_definitions( -DHAVE_CAIRO )
	endif ( OPT_ENABLE_CAIRO )

	if( OPT_ENABLE_GLUT )
		add_definitions( -DHAVE_GLUT )
	endif ( OPT_ENABLE_GLUT )

	if( LEGACYAPPS_PATH )
		add_definitions( -DHAVE_LEGACYAPPS )
	endif ( LEGACYAPPS_PATH )

	# Add SHAWN flag, to distinguish code needed by Shawn from code
	# that should be excluded, when an App is used by another program

	add_definitions( -DSHAWN )

	# Add flags for expat
	
	add_definitions( -DHAVE_EXPAT_CONFIG_H )
	add_definitions( -DXML_STATIC )


#=============
# Configure shawn apps modules
#=============

	# create new module config header files
	file( WRITE ${CMAKE_BINARY_DIR}/_apps_init_cmake.h "" )
	file( WRITE ${CMAKE_BINARY_DIR}/_apps_enable_cmake.h "#include \"shawn_config.h\"\n" )

	set( appsType APPS )
	set( appsPath apps )
	set( outFileEnabled ../buildfiles/_apps_enable_cmake.h )
	set( outFileInit ../buildfiles/_apps_init_cmake.h )

	include( ModuleCfg.cmake )


#=============
# Configure shawn legacyapps modules
#=============
	
	if ( LEGACYAPPS_PATH )

		# create new module config header files
		file( WRITE ${CMAKE_BINARY_DIR}/_legacyapps_init_cmake.h "" )
		file( WRITE ${CMAKE_BINARY_DIR}/_legacyapps_enable_cmake.h "#include \"shawn_config.h\"\n" )

		set( appsType LEGACYAPPS )
		set( appsPath ${LEGACYAPPS_PATH} )
		set( outFileEnabled _legacyapps_enable_cmake.h )
		set( outFileInit _legacyapps_init_cmake.h )

		include( ModuleCfg.cmake )
		
		#=============
		# When the legacyapps are used, we need to give the compiler
		# an additional include directory, because legacyapps sources
		# are outside of the build tree. This way, the old include
		# directives like
		#
		#   #include "legacyapps/marathon_net/marathon_world_factory.h"
		#
		# can still be used.
		#
		# Important: legacyapps sources must not be in the root
		#            directory, at least in one subdir like
		#            "/legacyapps/*.*", not "/*.*"!
		#=============
		
		include_directories( ${LEGACYAPPS_PATH}/.. )
		
	endif ( LEGACYAPPS_PATH )


#=============
# Option for creating a new module
#=============

	include( NewModule.cmake )


#=============
# Build target
#=============

	IF(BUILD_LIB_WITH_MAIN)
		add_library( shawnlib ${shawnSources} ${shawnSourcesLib} )
	ELSE(BUILD_LIB_WITH_MAIN)
		add_library( shawnlib ${shawnSourcesLib} )
		add_executable( shawn ${shawnSources} )
	ENDIF(BUILD_LIB_WITH_MAIN)


#=============
# Link libraries to the shawn target
#=============

	if ( WIN32 AND NOT CYGWIN  )
		set ( SYSTEM_LIBS ws2_32.lib )

		if ( OPT_ENABLE_CGAL )
			set ( LIB_CGAL CGAL.lib )
	  	endif ( OPT_ENABLE_CGAL )

		if ( LIB_PATH_LOG4CXX )
			set ( LIB_LOGGING log4Cxx.lib apr.lib aprutil.lib xml.lib odbc32.lib mswsock.lib)
		endif ( LIB_PATH_LOG4CXX )

		if ( OPT_ENABLE_CAIRO )
			set (LIB_CAIRO cairo.lib)
		endif ( OPT_ENABLE_CAIRO )

		if ( OPT_ENABLE_GLUT )
			set ( LIB_GLUT OpenGL32.Lib glut32.lib )
		endif ( OPT_ENABLE_GLUT )

		if ( OPT_ENABLE_BOOST )
			set ( LIB_BOOST )
		endif ( OPT_ENABLE_BOOST )

	else ( WIN32 AND NOT CYGWIN  )
		set ( SYSTEM_LIBS dl )

		if ( OPT_ENABLE_CGAL )
			set ( LIB_CGAL CGAL)
		endif ( OPT_ENABLE_CGAL )

		if ( LIB_PATH_LOG4CXX )
			set ( LIB_LOGGING log4cxx )
		endif ( LIB_PATH_LOG4CXX )

		if ( OPT_ENABLE_CAIRO )
			set (
				LIB_CAIRO cairo pixman-1 png freetype fontconfig z
			)
		endif ( OPT_ENABLE_CAIRO )

		if ( OPT_ENABLE_BOOST )
			if ( CYGWIN )
				set ( LIB_BOOST boost_regex-gcc-mt boost_date_time-gcc-mt boost_thread-gcc-mt boost_system-gcc-mt )
			else ( CYGWIN )
				set ( LIB_BOOST boost_regex-mt boost_date_time-mt boost_thread-mt boost_system-mt )
			endif ( CYGWIN )
		endif ( OPT_ENABLE_BOOST )

		if ( OPT_ENABLE_GLUT AND APPLE)
			set ( LIB_GLUT GL glu glut )
                        SET_TARGET_PROPERTIES( shawn PROPERTIES LINK_FLAGS "-dylib_file /System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib:/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib" )
		endif ( OPT_ENABLE_GLUT AND APPLE )

		if ( OPT_ENABLE_GLUT AND NOT APPLE)
			set ( LIB_GLUT GL glut )
		endif ( OPT_ENABLE_GLUT AND NOT APPLE )

	endif ( WIN32 AND NOT CYGWIN  )

	IF(NOT BUILD_LIB_WITH_MAIN)
		target_link_libraries( shawn shawnlib ${SYSTEM_LIBS} ${LIB_CGAL} ${LIB_LOGGING} 
			${LIB_CAIRO} ${LIB_BOOST} ${LIB_GLUT} ${SHAWN_LINK_LIBS} )
	ENDIF(NOT BUILD_LIB_WITH_MAIN)
