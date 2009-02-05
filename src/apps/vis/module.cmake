#=============
# Shawn module configuration for cmake build system
#=============

    OPTION(OPT_ENABLE_CAIRO "Enable Cairo library support" OFF)

    if ( OPT_ENABLE_CAIRO )
      set ( DEFAULT_CAIRO_ROOT )
      set ( LIB_PATH_CAIRO   CACHE PATH "Path to CAIRO library" )
      set ( INCLUDE_PATH_CAIRO   CACHE PATH "Path to CAIRO includes" )
	set ( INCLUDE_PATH_BOOST   CACHE PATH "Path to Boost include path" )
	set ( LIB_PATH_BOOST   CACHE PATH "Path to BOOST library" )
	link_directories( ${LIB_PATH_CAIRO} )
	link_directories( ${LIB_PATH_BOOST} )
      include_directories ( ${INCLUDE_PATH_CAIRO} )
      include_directories ( ${INCLUDE_PATH_BOOST} )
      add_definitions( -DHAVE_CAIRO ) 
      if ( INCLUDE_PATH_BOOST )
         add_definitions(-DHAVE_BOOST -DHAVE_BOOST_REGEX )
      endif ( INCLUDE_PATH_BOOST )
    endif ( OPT_ENABLE_CAIRO )

    if (  WIN32 AND NOT CYGWIN  )
        if ( OPT_ENABLE_CAIRO )
            if ( LIB_PATH_CAIRO )
					 set ( SHAWN_LINK_LIBS ${SHAWN_LINK_LIBS} cairo.lib )
            endif ( LIB_PATH_CAIRO )
        endif ( OPT_ENABLE_CAIRO )
    else (  WIN32 AND NOT CYGWIN  )
        if ( OPT_ENABLE_CAIRO )
            if ( LIB_PATH_CAIRO )
				    # $SHAWN_LINK_LIBS is used in src/CMakeLists.txt at
				    # the end of the file when calling
				    # "target_link_libraries". It can't be used here,
				    # because "add_executable" must be called first.
					 set ( SHAWN_LINK_LIBS ${SHAWN_LINK_LIBS} cairo pixman-1 png freetype fontconfig z 
						 	boost_regex boost_date_time boost_thread GL glut )
#boost_regex-gcc-mt )
            endif ( LIB_PATH_CAIRO )
        endif ( OPT_ENABLE_CAIRO )
    endif (  WIN32 AND NOT CYGWIN  )

# Name of this module

	set ( moduleName VIS )

# Default status (ON/OFF)

	set ( moduleStatus OFF )

# List of libraries needed by this module, seperated by white space

	set ( moduleLibs  )