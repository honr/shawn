#=============
# Shawn module configuration for cmake build system
#=============

    OPTION(OPT_ENABLE_CAIRO "Enable Cairo library support" OFF)

    if ( OPT_ENABLE_CAIRO )
        if ( WIN32 AND NOT CYGWIN )
            set ( DEFAULT_CAIRO_ROOT )
            set ( LIB_PATH_CAIRO   CACHE PATH "Path to CAIRO library" )
            set ( INCLUDE_PATH_CAIRO   CACHE PATH "Path to CAIRO includes" )
        else ( WIN32 AND NOT CYGWIN )
            set ( DEFAULT_CAIRO_ROOT )
            set ( LIB_PATH_CAIRO   CACHE PATH "Path to CAIRO library" )
            set ( INCLUDE_PATH_CAIRO   CACHE PATH "Path to CAIRO includes" )
        endif ( WIN32 AND NOT CYGWIN )
        link_directories( ${LIB_PATH_CAIRO} )
        include_directories ( ${INCLUDE_PATH_CAIRO} )
    endif ( OPT_ENABLE_CAIRO )

    if (  WIN32 AND NOT CYGWIN  )
        if ( OPT_ENABLE_CAIRO )
            if ( LIB_PATH_CAIRO )
                set ( LIB_CAIRO cairo.lib )
            endif ( LIB_PATH_CAIRO )
        endif ( OPT_ENABLE_CAIRO )
    else (  WIN32 AND NOT CYGWIN  )
        if ( OPT_ENABLE_CAIRO )
            if ( LIB_PATH_CAIRO )
                set ( LIB_CAIRO cairo pixman-1 png freetype fontconfig crypt z )
            endif ( LIB_PATH_CAIRO )
        endif ( OPT_ENABLE_CAIRO )
    endif (  WIN32 AND NOT CYGWIN  )

    IF(NOT BUILD_LIB_WITH_MAIN)
        target_link_libraries( shawn ${LIB_CAIRO} )
    ENDIF(NOT BUILD_LIB_WITH_MAIN)

    if( OPT_ENABLE_CAIRO )
       if ( LIB_PATH_CAIRO )
          add_definitions( -DHAVE_CAIRO )
       endif ( LIB_PATH_CAIRO )
    endif( OPT_ENABLE_CAIRO )

# Name of this module

	set ( moduleName VIS )

# Default status (ON/OFF)

	set ( moduleStatus OFF )

# List of libraries needed by this module, seperated by white space

	set ( moduleLibs  )