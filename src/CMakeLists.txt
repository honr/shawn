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

	OPTION(OPT_ENABLE_CGAL "Enable CGAL library support" OFF)

#=============
# Set the default paths for binary and lib build
#=============

	get_filename_component( EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../buildfiles ABSOLUTE CACHE )
	get_filename_component( LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../lib ABSOLUTE CACHE )

#=============
# Collect all source files recursively to be compiled
#=============

	file( GLOB_RECURSE shawnSourcesAppsCPP apps/*.cpp )
	file( GLOB_RECURSE shawnSourcesAppsH apps/*.h )

	file( GLOB_RECURSE shawnSourcesFrontendCPP frontend/*.cpp )
	file( GLOB_RECURSE shawnSourcesFrontendH frontend/*.h )
	
	file( GLOB_RECURSE shawnSourcesSysCPP sys/*.cpp )
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

	if ( OPT_ENABLE_CGAL )
	
		if ( WIN32 AND NOT CYGWIN )
	
			set ( DEFAULT_CGAL_ROOT   )
	
			set( LIB_PATH_CGAL   CACHE PATH "Path to CGAL library" )
	
	        set( INCLUDE_PATH_CGAL   CACHE PATH "Path to CGAL includes" )
	        set( INCLUDE_PATH_CGAL_COMPILER_CONFIG   CACHE PATH "Path to CGAL compiler config" )
	        set( INCLUDE_PATH_BOOST   CACHE PATH "Path to Boost include path" )
	
		else ( WIN32 AND NOT CYGWIN )
	
			set ( DEFAULT_CGAL_ROOT   )
	
			set( LIB_PATH_CGAL   CACHE PATH "Path to CGAL library" )
	
	        set( INCLUDE_PATH_CGAL   CACHE PATH "Path to CGAL includes" )
	        set( INCLUDE_PATH_CGAL_COMPILER_CONFIG   CACHE PATH "Path to CGAL compiler config" )
	        set( INCLUDE_PATH_BOOST   CACHE PATH "Path to Boost include path" )
	
		endif ( WIN32 AND NOT CYGWIN )
	
		link_directories( ${LIB_PATH_CGAL} )
						  
		include_directories( ./
							 ${INCLUDE_PATH_CGAL}
	                         ${INCLUDE_PATH_CGAL_COMPILER_CONFIG}
	                         ${INCLUDE_PATH_BOOST} )
	                         
	else ( OPT_ENABLE_CGAL )
	
		include_directories( ./ )
		
	endif ( OPT_ENABLE_CGAL )

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

	if (  WIN32 AND NOT CYGWIN  )

		
		if ( OPT_ENABLE_CGAL )
			
			if ( LIB_PATH_CGAL )
				set ( LIB_CGAL CGAL.lib )
			endif ( LIB_PATH_CGAL )
			
		endif ( OPT_ENABLE_CGAL )
		

		set ( SYSTEM_LIBS ws2_32.lib )

	else (  WIN32 AND NOT CYGWIN  )

		if ( OPT_ENABLE_CGAL )
			
			if ( LIB_PATH_CGAL )
				set ( LIB_CGAL libCGAL.a )
			endif ( LIB_PATH_CGAL )
			
		endif ( OPT_ENABLE_CGAL )

		set ( SYSTEM_LIBS dl )
		
	endif (  WIN32 AND NOT CYGWIN  )


	IF(NOT BUILD_LIB_WITH_MAIN)
		target_link_libraries( shawn shawnlib ${SYSTEM_LIBS} ${LIB_CGAL} )
	ENDIF(NOT BUILD_LIB_WITH_MAIN)

#=============
# Compiler definitions
#=============

	if( OPT_ENABLE_CGAL )
	if ( LIB_PATH_CGAL )
		add_definitions( -DHAVE_CGAL )
	endif ( LIB_PATH_CGAL )
	endif( OPT_ENABLE_CGAL )

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
	file( WRITE ../buildfiles/_apps_enable_cmake.h "#include \"shawn_config.h\"\n" )

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
		file( WRITE ../buildfiles/_legacyapps_enable_cmake.h "#include \"shawn_config.h\"\n" )

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
