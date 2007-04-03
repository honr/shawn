#==================================
#
# SHAWN build system
#
# Main input file for CMake
# 
#==================================

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
# Set the default paths for binary and lib build
#=============

	get_filename_component( EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../buildfiles ABSOLUTE CACHE )
	get_filename_component( LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../lib ABSOLUTE CACHE )


#=============
# Collect all source files recursively to be compiled
#=============

	file( GLOB_RECURSE shawnSourcesCPP *.cpp )
	file( GLOB_RECURSE shawnSourcesH *.h )

	#
	# Collect LEGACYAPPS sources if present
	#
	
	set( LEGACYAPPS_PATH "" CACHE PATH "Path to legacy apps. Leave blank to not use legacy apps." )
	if ( LEGACYAPPS_PATH )
		file( GLOB_RECURSE legacyappsSourcesCPP ${LEGACYAPPS_PATH}/*.cpp )
	endif ( LEGACYAPPS_PATH )


#=============
# Add collected source files to common source variables
#=============

	if ( shawnSourcesCPP )
		set( shawnSources ${shawnSourcesCPP} )
	endif ( shawnSourcesCPP )

	if ( shawnSourcesH )
		set( shawnSources ${shawnSources} ${shawnSourcesH} )
	endif ( shawnSourcesH )

	if ( shawnSourcesC )
		set( shawnSources ${shawnSources} ${shawnSourcesC} )
	endif ( shawnSourcesC )

	if ( legacyappsSourcesCPP )
		set( shawnSources ${shawnSources} ${legacyappsSourcesCPP} )
	endif ( legacyappsSourcesCPP )


#=============
# Add include and library directories for the compiler/linker
#=============

#
# Old default directories:
#
#	if ( WIN32 )
#
#		set ( DEFAULT_EXPAT_ROOT ../libs/expat-1.95.8 )
#		set ( DEFAULT_CGAL_ROOT  ../libs/CGAL-3.1 )
#
#		set( LIB_PATH_EXPAT ${DEFAULT_EXPAT_ROOT}/StaticLibs CACHE PATH "Path to expat library" )
#		set( LIB_PATH_CGAL ${DEFAULT_CGAL_ROOT}/lib/msvc7 CACHE PATH "Path to CGAL library" )
#
#       set( INCLUDE_PATH_EXPAT ${DEFAULT_EXPAT_ROOT}/source/lib CACHE PATH "Path to expat includes" )
#       set( INCLUDE_PATH_CGAL ${DEFAULT_CGAL_ROOT}/include CACHE PATH "Path to CGAL includes" )
#       set( INCLUDE_PATH_CGAL_COMPILER_CONFIG ${DEFAULT_CGAL_ROOT}/include/CGAL/config/msvc7 CACHE PATH "Path to CGAL compiler config" )
#       set( INCLUDE_PATH_BOOST ../libs/boost CACHE PATH "Path to Boost include path" )
#
#	else ( WIN32 )
#
#		set ( DEFAULT_EXPAT_ROOT ../libs/expat-1.95.8 )
#		set ( DEFAULT_CGAL_ROOT  ../libs/CGAL-3.1 )
#
#		set( LIB_PATH_EXPAT ${DEFAULT_EXPAT_ROOT}/.libs CACHE PATH "Path to expat library" )
#		set( LIB_PATH_CGAL ${DEFAULT_CGAL_ROOT}/lib/i686_Linux-2.4.26-om_g++-3.3.5 CACHE PATH "Path to CGAL library" )
#
#       set( INCLUDE_PATH_EXPAT ${DEFAULT_EXPAT_ROOT}/lib CACHE PATH "Path to expat includes" )
#       set( INCLUDE_PATH_CGAL ${DEFAULT_CGAL_ROOT}/include CACHE PATH "Path to CGAL includes" )
#       set( INCLUDE_PATH_CGAL_COMPILER_CONFIG ${DEFAULT_CGAL_ROOT}/include/CGAL/config/i686_Linux-2.4.26-om_g++-3.3.5 CACHE PATH "Path to CGAL compiler config" )
#       set( INCLUDE_PATH_BOOST ../libs/boost CACHE PATH "Path to Boost include path" )
#
#	endif ( WIN32 )


	if ( WIN32 )

		set ( DEFAULT_EXPAT_ROOT  )
		set ( DEFAULT_CGAL_ROOT   )

		set( LIB_PATH_EXPAT   CACHE PATH "Path to expat library" )
		set( LIB_PATH_CGAL   CACHE PATH "Path to CGAL library" )

        set( INCLUDE_PATH_EXPAT   CACHE PATH "Path to expat includes" )
        set( INCLUDE_PATH_CGAL   CACHE PATH "Path to CGAL includes" )
        set( INCLUDE_PATH_CGAL_COMPILER_CONFIG   CACHE PATH "Path to CGAL compiler config" )
        set( INCLUDE_PATH_BOOST   CACHE PATH "Path to Boost include path" )

	else ( WIN32 )

		set ( DEFAULT_EXPAT_ROOT  )
		set ( DEFAULT_CGAL_ROOT   )

		set( LIB_PATH_EXPAT   CACHE PATH "Path to expat library" )
		set( LIB_PATH_CGAL   CACHE PATH "Path to CGAL library" )

        set( INCLUDE_PATH_EXPAT   CACHE PATH "Path to expat includes" )
        set( INCLUDE_PATH_CGAL   CACHE PATH "Path to CGAL includes" )
        set( INCLUDE_PATH_CGAL_COMPILER_CONFIG   CACHE PATH "Path to CGAL compiler config" )
        set( INCLUDE_PATH_BOOST   CACHE PATH "Path to Boost include path" )

	endif ( WIN32 )


	link_directories( ${LIB_PATH_EXPAT}
					  ${LIB_PATH_CGAL} )

	include_directories( ./
						 ${INCLUDE_PATH_EXPAT}
						 ${INCLUDE_PATH_CGAL}
                         ${INCLUDE_PATH_CGAL_COMPILER_CONFIG}
                         ${INCLUDE_PATH_BOOST} )


#=============
# Build target
#=============

	add_executable( shawn ${shawnSources} )


#=============
# Link libraries to the shawn target
#=============

	if ( WIN32 )

		if ( LIB_PATH_EXPAT )
			set ( LIB_EXPAT libexpatMT.lib )
		endif ( LIB_PATH_EXPAT )

		if ( LIB_PATH_CGAL )
			set ( LIB_CGAL CGAL.lib )
		endif ( LIB_PATH_CGAL )

		set ( SYSTEM_LIBS ws2_32.lib )

	else ( WIN32 )

		if ( LIB_PATH_EXPAT )
			set ( LIB_EXPAT expat )
		endif ( LIB_PATH_EXPAT )

		if ( LIB_PATH_CGAL )
			set ( LIB_CGAL CGAL )
		endif ( LIB_PATH_CGAL )

		set ( SYSTEM_LIBS dl )
		
	endif ( WIN32 )

	target_link_libraries( shawn ${SYSTEM_LIBS} ${LIB_EXPAT} ${LIB_CGAL} )


#=============
# Check directories and give a warning, if one or more paths are incorrect
#=============

	set ( incorrectPaths  )

	# Only check paths if the current configure run is not the first
	# run, so the warning messages don't appear the first time the
	# user starts CMake

	if ( firstRunDone )

		if ( LIB_PATH_EXPAT AND NOT EXISTS ${LIB_PATH_EXPAT}/${LIB_EXPAT} )
			set ( incorrectPaths "${incorrectPaths} [LIB_PATH_EXPAT] Can't find '${LIB_EXPAT}' in '${LIB_PATH_EXPAT}'\n" )
		endif ( LIB_PATH_EXPAT AND NOT EXISTS ${LIB_PATH_EXPAT}/${LIB_EXPAT} )
	
		if ( LIB_PATH_CGAL AND NOT EXISTS ${LIB_PATH_CGAL}/${LIB_CGAL} )
			set ( incorrectPaths "${incorrectPaths} [LIB_PATH_CGAL] Can't find '${LIB_CGAL}' in '${LIB_PATH_CGAL}'\n" )
		endif ( LIB_PATH_CGAL AND NOT EXISTS ${LIB_PATH_CGAL}/${LIB_CGAL} )

		if ( INCLUDE_PATH_EXPAT AND NOT EXISTS ${INCLUDE_PATH_EXPAT}/expat.h )
			set ( incorrectPaths "${incorrectPaths} [INCLUDE_PATH_EXPAT] Can't find 'expat.h' in '${INCLUDE_PATH_EXPAT}'\n" )
		endif ( INCLUDE_PATH_EXPAT AND NOT EXISTS ${INCLUDE_PATH_EXPAT}/expat.h )

		if ( INCLUDE_PATH_CGAL AND NOT EXISTS ${INCLUDE_PATH_CGAL}/CGAL/basic.h )
			set ( incorrectPaths "${incorrectPaths} [INCLUDE_PATH_CGAL] Can't find 'CGAL/basic.h' in '${INCLUDE_PATH_CGAL}'\n" )
		endif ( INCLUDE_PATH_CGAL AND NOT EXISTS ${INCLUDE_PATH_CGAL}/CGAL/basic.h )

		if ( INCLUDE_PATH_CGAL_COMPILER_CONFIG AND NOT EXISTS ${INCLUDE_PATH_CGAL_COMPILER_CONFIG}/CGAL/compiler_config.h )
			set ( incorrectPaths "${incorrectPaths} [INCLUDE_PATH_CGAL_COMPILER_CONFIG] Can't find 'CGAL/compiler_config.h' in '${INCLUDE_PATH_CGAL_COMPILER_CONFIG}'\n" )
		endif ( INCLUDE_PATH_CGAL_COMPILER_CONFIG AND NOT EXISTS ${INCLUDE_PATH_CGAL_COMPILER_CONFIG}/CGAL/compiler_config.h )

		if ( INCLUDE_PATH_BOOST AND NOT EXISTS ${INCLUDE_PATH_BOOST}/boost/config.hpp )
			set ( incorrectPaths "${incorrectPaths} [INCLUDE_PATH_BOOST] Can't find '/boost/config.hpp' in '${INCLUDE_PATH_BOOST}'\n" )
		endif ( INCLUDE_PATH_BOOST AND NOT EXISTS ${INCLUDE_PATH_BOOST}/boost/config.hpp )
		
		if ( incorrectPaths )
			message( "Warning! The following paths seem to be incorrect. Shawn can't be compiled/linked with incorrect lib or include paths:\n\n${incorrectPaths}" )
		endif ( incorrectPaths )
	
	endif ( firstRunDone )
	
	set( firstRunDone TRUE CACHE INTERNAL firstRunDone )


#=============
# Compiler definitions
#=============

	if ( LIB_PATH_EXPAT )
		add_definitions( -DHAVE_EXPAT )
		add_definitions( -DXML_STATIC )
	endif ( LIB_PATH_EXPAT )
	
	if ( LIB_PATH_CGAL )
		add_definitions( -DHAVE_CGAL )
	endif ( LIB_PATH_CGAL )

	if ( LEGACYAPPS_PATH )
		add_definitions( -DHAVE_LEGACYAPPS )
	endif ( LEGACYAPPS_PATH )


	# Add SHAWN flag, to distinguish code needed by Shawn from code
	# that should be excluded, when an App is used by another program

	add_definitions( -DSHAWN )


#=============
# Configure shawn apps modules
#=============

	# create new module config header files
	file( WRITE ../buildfiles/_apps_init_cmake.h "" )
	file( WRITE ../buildfiles/_apps_enable_cmake.h "" )

	set( appsType APPS )
	set( appsPath apps )
	set( outFileEnabled _apps_enable_cmake.h )
	set( outFileInit _apps_init_cmake.h )

	include( ModuleCfg.cmake )


#=============
# Configure shawn legacyapps modules
#=============
	
	if ( LEGACYAPPS_PATH )

		# create new module config header files
		file( WRITE ../buildfiles/_legacyapps_init_cmake.h "" )
		file( WRITE ../buildfiles/_legacyapps_enable_cmake.h "" )

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
